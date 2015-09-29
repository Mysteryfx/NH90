#ifndef __ED_DIR_H__
#define __ED_DIR_H__


#include "config.h"

#include <ed/string.h>

#ifndef _WINDOWS
#include <sys/types.h>
#include <dirent.h>
#else
#include <io.h>
#endif

#ifndef ED_CORE_EXPORT
	#define ED_CORE_EXPORT ED_DLL_EXPORT
#endif // ED_CORE_EXPORT


namespace io {


class DirectoryIterator;


/// Filter unnneded files for directory iterator
class ED_CORE_EXPORT FileFilter
{
    public:
        /// Filter must die
        virtual ~FileFilter() { };

        /// Returns false if file must be filtered out.
        /// \param iterator parent iterator.
        /// \param fileName name of current file.
        virtual bool isGoodFile(DirectoryIterator &iterator,
                const ed::string& fileName) = 0;
};

template class ED_CORE_EXPORT std::allocator<char>;
template class ED_CORE_EXPORT std::basic_string<char,std::char_traits<char>,std::allocator<char> >;

/// Filter unnneded files for directory iterator
class ED_CORE_EXPORT FileSuffixFilter: public FileFilter
{
    private:
        /// Suffix of file.
        ed::string suffix;

    public:
        /// Create suffix filter.
        /// For example use .txt suffix to receive list of text files.
        /// \param suffix suffix for filtering.
        FileSuffixFilter(const ed::string &suffix);

    public:
        /// Returns false if file must be filtered out.
        /// Allow only files with specified suffix.
        /// \param iterator parent iterator.
        /// \param fileName name of current file.
        virtual bool isGoodFile(DirectoryIterator &iterator,
                const ed::string& fileName);
};


/// Iterate through files in directory
class ED_CORE_EXPORT DirectoryIterator
{
    private:

#if !defined(_WIN32) && !defined(_WIN64) 

        /// UNIX directory handler
        DIR *dir;

        /// FILE name record
        struct dirent *dirent;

#else
        /// Directory handler
        intptr_t hndl;

        /// Readed file
        struct _finddata_t fileInfo;

#endif

        /// Name of directory
        ed::string dirName;

        /// Filter to hide unwanted files.
        FileFilter *filter;

    public:
        /// Create directory iterator with filter.
        /// Filter will be destroyed by iterator destructor.
        /// \param directory name of directory to iterate.
        /// \param filter filter for hiding unwanted files.
        DirectoryIterator(const ed::string &directory,
                FileFilter *filter=NULL);

        /// Create directory iterator with suffix filter.
        /// \param directory name of directory to iterate.
        /// \param suffix suffix of files
        DirectoryIterator(const ed::string &directory,
                const ed::string &suffix);

        /// Destroys iterator and associated filter.
        ~DirectoryIterator();

    public:
        /// Returns true if more files available.
        bool hasNext();

        /// Returns name of file and move iterator to next position.
        /// Name of file includes directory!
        ed::string next();

        /// Return name of traversed directory.
        const ed::string& getDirectoryName() const { return dirName; }

    private:
        /// Move iterator to next file.
        void advanceToNext();

        /// Initialize iterator.
        void init();
};


/// Returns true if path is directory or false otherwise.
ED_CORE_EXPORT bool isDirectory(const ed::string &file);

/// Create all subdirectories.
ED_CORE_EXPORT void createDirs(const ed::string &dirs);

/// Returns parent direcoty.
ED_CORE_EXPORT ed::string getParentDir(const ed::string &path);

/// Returns true if file wis given name exists.
ED_CORE_EXPORT bool fileExists(const ed::string &file);

/// Returns name of file without directory.
ED_CORE_EXPORT ed::string getFileName(const ed::string &path);

/// Return path to user's home directory.
ED_CORE_EXPORT ed::string getHomeDir();

}

#endif

