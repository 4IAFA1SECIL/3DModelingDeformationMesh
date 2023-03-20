#include <windows.h>
#include <commdlg.h>

#include "file_dialog.hpp"

namespace file_dialog {

    // Function to create a file dialog and return the selected file path
    std::string openFileDialog(HWND hwnd, bool save) {
        OPENFILENAME ofn;       // structure to hold the file dialog options
        char szFile[260];       // buffer to hold the selected file path
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "Objet Files (*.obj)\0*.obj\0 All Files (*.*)\0*.*\0";   // filter to show all files
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING | OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

        if (save) {
            return GetSaveFileName(&ofn) == TRUE ? std::string(szFile) : "";
        } else {
            return GetOpenFileName(&ofn) == TRUE ? std::string(szFile) : "";
        }
    }

    std::string get_file(bool save) {
        HWND hwnd = NULL;   // set the parent window
        std::string filePath = openFileDialog(hwnd, save);   
        if (!filePath.empty()) {
        return filePath;
        }
        return "";
    }

}