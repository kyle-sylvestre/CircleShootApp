#include "Zuma_Prefix.pch"

#include "CircleShootApp.h"
#include <SDL2/SDL.h>

#if defined(__APPLE__)
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
void PlatformInit()
{
    SDL_SetHint(SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK, "1");
    NSString *pathWithTilde = @"~/Library/Application Support/ZumaPortable";
    NSString *fullPath = [pathWithTilde stringByExpandingTildeInPath];
    const char *path = [fullPath UTF8String];
    Sexy::MkDir(path);
    
    for (;;)
    {
        // read zuma file location
        std::string rpath;
        std::string filename = Sexy::StrFormat("%s/gamefolder.txt", path);
        std::ifstream file(filename, std::ios::binary); // open in binary mode
        if (file)
        {
            std::ostringstream contents;
            contents << file.rdbuf(); // read entire file into stream
            rpath = contents.str();
            
            // remove line endings
            while (rpath.size() && (rpath.back() == '\n' || rpath.back() == '\r'))
                rpath.pop_back();
        }

        if (Sexy::FileExists((rpath + "/properties/resources.xml").c_str()))
        {
            Sexy::SetResourceFolder(rpath);
            Sexy::ChDir(rpath);
            break;
        }
        else
        {
            // THIS WILL NOT WORK AS CONSOLE APP, NEEDS TO BE APP BUNDLE
            NSOpenPanel* openDlg = [NSOpenPanel openPanel];
            [openDlg setMessage:@"Select Zuma Deluxe Folder"];
            [openDlg setCanChooseDirectories:YES];
            [openDlg setCanChooseFiles:NO];
            [openDlg setAllowsMultipleSelection:NO];
            [openDlg setDirectoryURL:[NSURL URLWithString:[NSString stringWithUTF8String:"."] ] ];
            [openDlg setTreatsFilePackagesAsDirectories:YES];
            auto result = [openDlg runModal];
            if (result == NSModalResponseOK)
            {
                std::string path = [[[[openDlg URLs] objectAtIndex:0] path] UTF8String];
                
                // fixup path for macOS demo version
                std::string mac_demo_path = Sexy::StrFormat("%s/Contents/Resources/Zuma Deluxe.app/Contents/Resources", path.c_str());
                if (Sexy::FileExists(mac_demo_path))
                {
                    path = mac_demo_path;
                }
                
                // fixup path for macOS full version
                std::string mac_full_path = Sexy::StrFormat("%s/Contents/Resources", path.c_str());
                if (Sexy::FileExists(mac_full_path))
                {
                    path = mac_full_path;
                }
                
                std::ofstream outFile(filename, std::ios::binary);
                if (outFile)
                {
                    outFile.write(path.c_str(), path.size());
                    outFile.close();
                }
            }
            else
            {
                exit(0);
            }
        }
    }
}
#elif defined(_WIN32)
#include <windows.h>
#include <shlobj.h>   // for SHGetFolderPathA, SHBrowseForFolder, etc.
#include <string>
#include <fstream>
#include <sstream>

bool PickFolderLegacy(std::string& outPath, const std::string& initialFolder = ".")
{
    bool result = false;
    BROWSEINFOA bi = {0};
    char path[MAX_PATH] = {0};

    bi.hwndOwner = nullptr;
    bi.pidlRoot = nullptr; // Start from "My Computer"
    bi.pszDisplayName = path;
    bi.lpszTitle = "Select Zuma Deluxe Folder";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_EDITBOX;

    // Optionally set initial folder using a PIDL
    LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
    if (pidl != nullptr)
    {
        if (SHGetPathFromIDListA(pidl, path))
        {
            outPath = path;
            result = true;
        }
        CoTaskMemFree(pidl);
    }
    return result;
}

void PlatformInit()
{
    // check if running on wine
    {
        HMODULE hntdll = GetModuleHandle("ntdll.dll");
        if(hntdll)
        {
            FARPROC wine_get_version = GetProcAddress(hntdll, "wine_get_version");
            if (wine_get_version)
            {
                // wasapi causes static/crackle sounds, force directsound
                SDL_SetHint(SDL_HINT_AUDIODRIVER, "directsound");
            }
        }
    }

    // 1. Create settings folder in AppData\Roaming\ZumaPortable
    char appDataPath[MAX_PATH] = {};
    if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_APPDATA, nullptr, 0, appDataPath)))
    {
        std::string path = std::string(appDataPath) + "\\ZumaPortable";
        Sexy::MkDir(path.c_str());

        std::string gameFolderFile = path + "\\gamefolder.txt";
        std::string rpath;

        for (;;)
        {
            // 2. Try reading saved path
            std::ifstream file(gameFolderFile, std::ios::binary);
            if (file)
            {
                std::ostringstream contents;
                contents << file.rdbuf();
                rpath = contents.str();
                while (!rpath.empty() && (rpath.back() == '\r' || rpath.back() == '\n'))
                    rpath.pop_back();
            }

            // 3. Check if the game folder is valid
            if (Sexy::FileExists((rpath + "\\properties\\resources.xml").c_str()))
            {
                Sexy::SetResourceFolder(rpath);
                Sexy::ChDir(rpath);
                break;
            }
            else
            {
                // 4. Show legacy folder selection dialog
                if (!PickFolderLegacy(rpath))
                {
                    exit(0); // user canceled
                }

                // Save path to file
                std::ofstream outFile(gameFolderFile, std::ios::binary);
                if (outFile)
                {
                    outFile.write(rpath.c_str(), rpath.size());
                    outFile.close();
                }
            }
        }
    }
    else
    {
        MessageBoxA(nullptr, "Failed to get AppData folder.", "Error", MB_OK | MB_ICONERROR);
        exit(0);
    }
}

#else
void PlatformInit() 
{
}
#endif

int main(int argc, char *argv[])
{
    Sexy::CircleShootApp app;
    PlatformInit();

    app.Init();
    app.Start();
    app.Shutdown();
    
    return 0;
}
