#ifndef _ED_set_workdir_hpp_
#define _ED_set_workdir_hpp_

#include <windows.h>
#include "Common/Unicode.h"

namespace ed {

    inline ed::string _makepath(const ed::string& parent, const ed::string child)
    {
        ed::string path = parent;
        if (!path.empty() && path.find_first_of("/\\", path.size()-1)==ed::string::npos)
            path += '\\';
        path += child;
        return path;
    }
    
    inline bool _exists(const ed::string& basedir, const ed::string& child)
    {
        const ed::string path = _makepath(basedir, child);
        return (GetFileAttributesW(Common::iconv_utf8_to_wchar(path).c_str()) != INVALID_FILE_ATTRIBUTES);
    }
        
    inline ed::string _dirname(const ed::string& path)
    {
        // check all but last char (which is either slash or does not matter)
        size_t sep = path.find_last_of("/\\", path.size()-2);
        if (sep == ed::string::npos)
            return ed::string();
        return path.substr(0, sep+1);
    }
    
    inline
    bool _set_basedir(ed::string& basedir, const ed::string& target = "Bazar")
    {
        if (!_exists(basedir, target))
        {
            wchar_t bindir[512];
            DWORD len = GetModuleFileNameW(NULL, bindir, sizeof(bindir)/sizeof(bindir[0]));
            if (len == 0)
                return false;
            basedir = _dirname(Common::iconv_utf8_from_wchar(bindir));
            while (!_exists(basedir, target))
            {
                if (basedir.empty())
                    return false;
                basedir = _dirname(basedir);
            }
        }
        if (!basedir.empty())
        {
            if (!SetCurrentDirectoryW(Common::iconv_utf8_to_wchar(basedir).c_str()))
                return false;
        }
        wchar_t curdir[512];
        if (!GetCurrentDirectoryW(sizeof(curdir)/sizeof(curdir[0]), curdir))
            return false;
        basedir = Common::iconv_utf8_from_wchar(curdir);
        return true;
    }

    inline
    ed::string _get_branchname(const ed::string& basedir, const ed::string& tagfile=ed::string())
    {
        const ed::string tagpath = _makepath(basedir, tagfile.empty() ? "dcs_variant.txt" : tagfile);
        HANDLE h = CreateFileW(Common::iconv_utf8_to_wchar(tagpath).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (h == INVALID_HANDLE_VALUE)
            return "";

        char tag[64];
        DWORD tagsize = 0;
        BOOL res = ReadFile(h, tag, (DWORD)sizeof(tag), &tagsize, NULL);
        CloseHandle(h);
        if (!res)
            return "";

        // restrict to base64url charset
        ed::string result;
        result.reserve(tagsize);
        for (int i=0; i<tagsize; ++i)
            if (isalnum(tag[i]) || tag[i] == '_' || tag[i] == '-')
                result += tag[i];

        return result;
    }

    inline
    bool _set_writesubdir(ed::string& writesubdir, const ed::string& basedir, const ed::string& defvalue="DCS", const ed::string& tagfile=ed::string())
    {
        if (!writesubdir.empty())
            return false;

        const ed::string branchname = _get_branchname(basedir, tagfile);
        if (branchname.empty())
            writesubdir = defvalue;
        else
            writesubdir = defvalue + "." + branchname;
        return true;
    }
    
} // namespace ed

#endif /* _ED_set_workdir_hpp_ */
