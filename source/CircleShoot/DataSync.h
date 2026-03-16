#ifndef __DATASYNC_H__
#define __DATASYNC_H__

#include <SexyAppFramework/Common.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

namespace Sexy
{
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class DataReaderException : public std::exception
    {
    };

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class DataReader
    {
    public:
        FILE *mFileHandle;
        const void *mMemoryHandle;
        uint32_t mMemoryLength;
        uint32_t mMemoryPosition;
        bool mDeallocate;

        explicit DataReader();
        virtual ~DataReader();

        void OpenMemory(const void *theMemory, uint32_t theLength, bool deallocate);
        bool OpenFile(const std::string &theFileName);
        void Close();

        void ReadBytes(void *theBuffer, uint32_t theLength);
        uint32_t ReadLong();
        ushort ReadShort();
        uchar ReadByte();
        bool ReadBool();
        float ReadFloat();
        void ReadString(std::string &theString);
    };

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class DataWriter
    {
    public:
        FILE *mFileHandle;
        void *mMemoryHandle;
        uint32_t mMemoryPosition;
        uint32_t mMemoryLength;

        explicit DataWriter();
        virtual ~DataWriter();

        void OpenMemory(uint32_t theLength);
        void Close();
        void EnsureCapacity(uint32_t theLength);

        void WriteBytes(const void *theBuffer, uint32_t theLength);
        void WriteLong(uint32_t theValue);
        void WriteShort(ushort theValue);
        void WriteByte(uchar theValue);
        void WriteBool(bool theValue);
        void WriteFloat(float theValue);
        void WriteString(const std::string &theString);
    };

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class DataSync
    {
    public:
        typedef std::map<void *, int> PointerToIntMap;
        typedef std::map<int, void *> IntToPointerMap;

        DataReader *mReader;
        DataWriter *mWriter;
        int mVersion;

        explicit DataSync(DataReader &reader);
        explicit DataSync(DataWriter &writer);

        void SyncBytes(void *theValue, uint32_t theSize);
        void SyncLong(int &theValue);
        void SyncLong(unsigned int &theValue);
        //void SyncLong(uint32_t &theValue);
        void SyncShort(int &theValue);
        void SyncShort(ushort &theValue);
        void SyncSShort(int &theValue);
        void SyncByte(int &theValue);
        void SyncByte(ushort &theValue);
        void SyncBool(bool &theValue);
        void SyncFloat(float &theValue);
        void SyncString(std::string &theValue);

        void SyncPointers();
        void SyncPointer(void **thePtr);

        void RegisterPointer(void *thePtr);
        void ResetPointerTable();

    private:
        PointerToIntMap mPointerToIntMap;
        IntToPointerMap mIntToPointerMap;
        std::vector<void **> mPointerSyncList;
        int mCurPointerIndex;
    };

    template <typename TContainer>
    void DataSync_SyncSTLContainer(DataSync &theSync, TContainer &theValue)
    {
        typedef TContainer type;

        if (theSync.mReader != NULL)
        {
            theValue.clear();
            uint32_t aLength = theSync.mReader->ReadLong();
            for (uint32_t i = 0; i < aLength; i++)
            {
                typename type::value_type aValue;
                theValue.push_back(aValue);

                typename type::value_type &aRef = theValue.back();
                aRef.SyncState(theSync);
            }
        }
        else
        {
            theSync.mWriter->WriteLong(theValue.size());
            for (typename type::iterator i = theValue.begin(); i != theValue.end(); ++i)
            {
                i->SyncState(theSync);
            }
        }
    }
};

#endif
