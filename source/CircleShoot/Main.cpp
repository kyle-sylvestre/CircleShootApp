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
