/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Dune Legacy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dune Legacy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <FileClasses/Pakfile.h>
#include <misc/exceptions.h>
#include <misc/SDL2pp.h>

#include <stdlib.h>
#include <string>


/// Constructor for Pakfile
/**
    The PAK-File to be read/write is specified by the pakfilename-parameter. If write==false the file is opened for
    read-only and is closed in the destructor. If write==true the file is opened write-only and written out in the
    destructor.
    \param pakfilename  Filename of the *.pak-File.
    \param write        Specified if the PAK-File is opened for reading or writing (default is false).
*/
Pakfile::Pakfile(const std::string& pakfilename, bool write)
 : write(write), fPakFile(nullptr), filename(pakfilename), writeOutData(nullptr), numWriteOutData(0) {

    if(write == false) {
        // Open for reading
        if( (fPakFile = FileStream::Open(filename.c_str(), "rb")) == nullptr) {
            THROW(std::invalid_argument, "Pakfile::Pakfile(): Cannot open " + pakfilename + "!");
        }

        try {
            readIndex();
        } catch (std::exception&) {
            delete(fPakFile);
            throw;
        }
    } else {
        // Open for writing
        if( (fPakFile = FileStream::Open(filename.c_str(), "wb")) == nullptr) {
            THROW(std::invalid_argument, "Pakfile::Pakfile(): Cannot open " + pakfilename + "!");
        }
    }
}

/// Destructor
/**
    Closes the filehandle and releases all memory.
*/
Pakfile::~Pakfile()
{
    if(write == true) {
        // calculate header size
        int headersize = 0;
        for(unsigned int i = 0; i < fileEntries.size(); i++) {
            headersize += 4;
            headersize += fileEntries[i].filename.length() + 1;
        }
        headersize += 4;

        // write out header
        for(unsigned int i = 0; i < fileEntries.size(); i++) {
            #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            Uint32 startoffset = SDL_Swap32(fileEntries[i].startOffset + headersize);
            #else
            Uint32 startoffset = fileEntries[i].startOffset + headersize;
            #endif
            fPakFile->Write((char*) &startoffset,sizeof(Uint32),1);
            fPakFile->Write(fileEntries[i].filename.c_str(), fileEntries[i].filename.length() + 1,1);
        }
        Uint32 tmp = 0;
        fPakFile->Write((char*) &tmp, sizeof(Uint32), 1);

        // write out data
        fPakFile->Write(writeOutData,numWriteOutData,1);
    }

    if(fPakFile != nullptr) {
        delete(fPakFile);
    }

    if(writeOutData != nullptr) {
        free(writeOutData);
        writeOutData = nullptr;
        numWriteOutData = 0;
    }
}

/// Returns the name of the nth file in this pak-File.
/**
    Returns the name of the nth file in this pak-File. index specifies which file.
    \param  index   Index in pak-File
    \return name of the file specified by index
*/
const std::string& Pakfile::getFilename(unsigned int index) const {
    if(index >= fileEntries.size()) {
        THROW(std::invalid_argument, "Pakfile::getFilename(%ud): This Pakfile has only %ud entries!", index, fileEntries.size());
    }

    return fileEntries[index].filename;
}

/// Adds a file to this PAK-File
/**
    This methods adds the SDL_RWop File to this PAK-File. The used name is specified by filename. If the Pakfile
    is read-only this method has no effect.
    \param  rwop        Data to add (the SDL_RWop can be read-only but must support seeking)
    \param  filename    This is the filename the data is added with
*/
void Pakfile::addFile(AbstractStream* rwop, const std::string& filename) {
    if(write == false) {
        THROW(std::runtime_error, "Pakfile::addFile(): Pakfile is opened for read-only!");
    }

    if(rwop == nullptr) {
        THROW(std::invalid_argument, "Pakfile::addFile(): rwop==nullptr is not allowed!");
    }


    size_t filelength = static_cast<size_t>(rwop->Size());

    char* extendedBuffer;
    if((extendedBuffer = (char*) realloc(writeOutData,numWriteOutData+filelength)) == nullptr) {
        throw std::bad_alloc();
    } else {
        writeOutData = extendedBuffer;
    }

    if(rwop->Read(writeOutData + numWriteOutData,1,filelength) != filelength) {
        // revert the buffer to the original size
        char* shrinkedBuffer;
        if((shrinkedBuffer = (char*) realloc(writeOutData,numWriteOutData)) == nullptr) {
            // shrinking the buffer should not fail
            THROW(std::runtime_error, "Pakfile::addFile(): realloc failed!");
        }
        writeOutData = shrinkedBuffer;
        THROW(std::runtime_error, "Pakfile::addFile(): SDL_RWread failed!");
    }

    PakFileEntry newPakFileEntry;
    newPakFileEntry.startOffset = numWriteOutData;
    newPakFileEntry.endOffset = numWriteOutData + filelength - 1;
    newPakFileEntry.filename = filename;

    fileEntries.push_back(newPakFileEntry);

    numWriteOutData += filelength;

    rwop->Seek(0,SEEK_SET);
}

/// Opens a file in this PAK-File.
/**
    This method opens the file specified by filename. It is only allowed if the Pakfile is opened for reading.
    The returned SDL_RWops-structure can be used readonly with SDL_RWread, SDL_RWsize, SDL_RWseek and SDL_RWclose. No writing
    is supported.<br>
    NOTICE: The returned SDL_RWops-Structure is only valid as long as this Pakfile-Object exists. It gets
    invalid as soon as Pakfile:~Pakfile() is executed.
    \param  filename    The name of this file
    \return SDL_RWops for this file
*/
AbstractStream* Pakfile::openFile(const std::string& filename) {
    if(write == true) {
        THROW(std::runtime_error, "Pakfile::openFile(): Writing files is not supported!");
    }

    // find file
    int index = -1;
    for(unsigned int i=0;i<fileEntries.size();i++) {
        if(filename == fileEntries[i].filename) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        THROW(io_error, "Pakfile::openFile(): Cannot find file with name '%s' in this PAK file!", filename.c_str());
    }

    // alloc RWopData
    RWopData* pRWopData = new RWopData();

    pRWopData->curPakfile = this;
    pRWopData->fileOffset = 0;
    pRWopData->fileIndex = index;

    return new Stream(pRWopData);
}

bool Pakfile::exists(const std::string& filename) const {
    for(unsigned int i=0;i<fileEntries.size();i++) {
        if(filename == fileEntries[i].filename) {
            return true;
        }
    }

    return false;
}


size_t Pakfile::Stream::Read(void *ptr, size_t size, size_t n) {
    RWopData* pRWopData = this->pRWopData;
    if (pRWopData == nullptr) {
        return 0;
    }

    int bytes2read = size*n;

    Pakfile* pPakfile = pRWopData->curPakfile;
    if(pPakfile == nullptr) {
        return 0;
    }

    if(pRWopData->fileIndex >= pPakfile->fileEntries.size()) {
        return 0;
    }

    uint32_t readstartoffset = pPakfile->fileEntries[pRWopData->fileIndex].startOffset + pRWopData->fileOffset;

    if(readstartoffset > pPakfile->fileEntries[pRWopData->fileIndex].endOffset) {
        return 0;
    }

    if(readstartoffset + bytes2read > pPakfile->fileEntries[pRWopData->fileIndex].endOffset) {
        bytes2read = pPakfile->fileEntries[pRWopData->fileIndex].endOffset + 1 - readstartoffset;
        // round to last full block
        bytes2read /= size;
        bytes2read *= size;
        if(bytes2read == 0) {
            return 0;
        }
    }

    if(pPakfile->fPakFile->Seek(readstartoffset,SEEK_SET) < 0) {
        return 0;
    }

    if(pPakfile->fPakFile->Read(ptr,bytes2read,1) != 1) {
        return 0;
    }

    pRWopData->fileOffset += bytes2read;
    return bytes2read/size;
}

size_t Pakfile::Stream::Write(const void *ptr, size_t size, size_t n) {
    return 0;
}

Sint64 Pakfile::Stream::Size() {
    RWopData* pRWopData = this->pRWopData;
    if (pRWopData == nullptr) {
        return -1;
    }

    Pakfile* pPakfile = pRWopData->curPakfile;
    if(pPakfile == nullptr) {
        return -1;
    }

    if(pRWopData->fileIndex >= pPakfile->fileEntries.size()) {
        return -1;
    }

    return static_cast<Sint64>(pPakfile->fileEntries[pRWopData->fileIndex].endOffset - pPakfile->fileEntries[pRWopData->fileIndex].startOffset + 1);
}

Sint64 Pakfile::Stream::Seek(Sint64 offset, int whence) {
    RWopData* pRWopData = this->pRWopData;
    if (pRWopData == nullptr) {
        return -1;
    }

    Pakfile* pPakfile = pRWopData->curPakfile;
    if(pPakfile == nullptr) {
        return -1;
    }

    if(pRWopData->fileIndex >= pPakfile->fileEntries.size()) {
        return -1;
    }

    Sint64 newOffset;

    switch(whence) {
        case SEEK_SET:
        {
            newOffset = offset;
        } break;

        case SEEK_CUR:
        {
            newOffset = pRWopData->fileOffset + offset;
        } break;

        case SEEK_END:
        {
            newOffset = pPakfile->fileEntries[pRWopData->fileIndex].endOffset - pPakfile->fileEntries[pRWopData->fileIndex].startOffset + 1 + offset;
        } break;

        default:
        {
            return -1;
        } break;
    }

    if(newOffset > (pPakfile->fileEntries[pRWopData->fileIndex].endOffset - pPakfile->fileEntries[pRWopData->fileIndex].startOffset + 1)) {
        return -1;
    }

    pRWopData->fileOffset = static_cast<size_t>(newOffset);
    return newOffset;
}

int Pakfile::Stream::Close() {
    RWopData* pRWopData = this->pRWopData;
    if (pRWopData == nullptr) {
        return -1;
    }

    delete pRWopData;
    this->pRWopData = nullptr;
    return 0;
}

void Pakfile::readIndex()
{
    while(1) {
        PakFileEntry newEntry;

        if(fPakFile->Read((void*) &newEntry.startOffset, sizeof(newEntry.startOffset), 1) != 1) {
            THROW(std::runtime_error, "Pakfile::readIndex(): SDL_RWread() failed!");
        }

        //pak-files are always little endian encoded
        newEntry.startOffset = SDL_SwapLE32(newEntry.startOffset);
        newEntry.endOffset = 0;

        if(newEntry.startOffset == 0) {
            break;
        }

        while(1) {
            char tmp;
            if(fPakFile->Read(&tmp,1,1) != 1) {
                THROW(std::runtime_error, "Pakfile::readIndex(): SDL_RWread() failed!");
            }

            if(tmp == '\0') {
                break;
            } else {
                newEntry.filename += tmp;
            }
        }

        if(fileEntries.empty() == false) {
            fileEntries.back().endOffset = newEntry.startOffset - 1;
        }

        fileEntries.push_back(newEntry);
    }

    Sint64 filesize = fPakFile->Size();
    if(filesize < 0) {
        THROW(std::runtime_error, "Pakfile::readIndex(): SDL_RWsize() failed!");
    }

    fileEntries.back().endOffset = static_cast<size_t>(filesize) - 1;
}
