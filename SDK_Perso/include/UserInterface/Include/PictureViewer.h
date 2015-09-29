// PictureViewer.h

#ifndef __USERINTERFACE_PICTUREVIEWER_H__
#define __USERINTERFACE_PICTUREVIEWER_H__

#include "MultiColumnListBox.h"

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR PictureViewerParameters : 
						public MultiColumnListBoxParameters
{
	typedef MultiColumnListBoxParameters Base;	
public:
	PictureViewerParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(PictureViewerParameters)
	virtual void serialize(Serializer &);

	void read(const char* text, int size);

	ed::string			m_FolderName;
};


class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR PictureViewer : public MultiColumnListBox
{
	ed::string			m_FolderName;
private:
	bool			SetTextureName(Control* Element, const char* inPictFileName);
public:

	typedef MultiColumnListBox Base;
	typedef	PictureViewerParameters Parameters;

public:
	PictureViewer(const PictureViewerParameters& inParams);

	virtual bool	addString(const char* inPictFileName);	
	virtual void	setItemCaption(const char* inPictFileName, int column, int row);

	void			setFolderName(const char* inFolderName);
	void			FillFromFolder();
};

#endif	// __USERINTERFACE_PICTUREVIEWER_H__