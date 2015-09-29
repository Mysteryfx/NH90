/**
 * @file UI/Chart.h
 * @author Dmitry S. Baikov
 */
#ifndef GENUI_CHART_H
#define GENUI_CHART_H

#include "./Element.h"

namespace UI {


/**
 * Generic chart interface.
 * In every function either record or field number set to -1 means 'all'.
 */
class GENUI_API Chart : virtual public Element {
public:
	typedef Signal::Notifier<Chart*> Event;

	enum {
		Begin = 0, // for add/delRecords/Fields
		End = -1,

		All = -1 // for Selection
	};

	virtual void    clear() = 0;
    virtual void    resize(int count) { if (size() > count) delRecord(count, size() - count); 
        else if (size() < count) newRecord(End, count - size()); }

	/**
	 * Add new record(s) and get index
	 */
	virtual int newRecord(int before=End, int count=1) = 0;

	/**
	 * Add new field(s) and get index
	 */
	virtual int newField(int before=End, int count=1) = 0;

	/**
	 * Delete specified record(s)
	 */
	virtual bool delRecord(int row, int count=1) = 0;

	/**
	 * Delete specified field(s)
	 */
	virtual bool delField(int field, int count=1) = 0;

	/*
	 * Set cell value
	 */
	virtual bool setValue(int record, int field, const ed::string& val) = 0;
//	virtual bool setValue(int, int, int val) = 0;
//	virtual bool setValue(int, int, double) = 0;

	/*
	 * Get cell value
	 */
	virtual bool getValue(int record, int field, ed::string* val) const = 0;
//	virtual bool getValue(int, int, int* value) const = 0;
//	virtual bool getValue(int, int, double* val) const = 0;

	/**
	 * Get number of records
	 */
	virtual int allRecords() const = 0;
	int numRecords() const { return allRecords(); }
    virtual int size() const { return allRecords(); }

	/**
	 * Get number of fields
	 */
	virtual int allFields() const = 0;
	virtual int numFields() const { return allFields(); }

	/**
	 * Set selection
	 * @param record number of record -1=all
	 * @param field number of field -1=all
	 * @param extend should we add specified selection to the existent
	 */
	virtual bool setSelection(int record, int field=All, bool extend=false) = 0;

	/**
	 * Get selection
	 * @param record number of selected record -1=all
	 * @param field number of field -1=all
	 * @param next should we get next multiselection part
	 * or new selection from the very beginning
	 */
	virtual bool getSelection(int* record, int* field=0, bool next=false) const = 0;

	/**
	 * Selection changed notifier
	 */
	Event& selectionChanged() { return sig_selChanged; }

	/**
	 * Value changed notifier
	 */
	Event& valueChanged() { return sig_valChanged; }

private:
	Event sig_selChanged;
	Event sig_valChanged;

}; // class Chart

/* COM-like ptr typedef */
typedef Ptr<Chart> IChartPtr;


/*
 * Usage example:

Dialog {
	UI::IChartPtr chart;
	UI::IEditor editor;
};

void Dialog::init()
{
	chart = child( 123 );
	editor = child( 321 );

	for( int i=0; i<10; ++i )
		chart->setValue( chart->newRecord(), 0, i );

	chart->setSelection( 0 );
	chart->selectionChanged().attach( this, &Dialog::onChartSelect );
	onChartSelect();
}

void Dialog::onChartSelect()
{
	int r;
	chart->getSelection( &r );
	ed::string str;
	chart->getValue( r, 0, &str );
	editor->setText( str );
} 

void Dialog::close()
{
	chart->selectionChanged().detach( this );
	chart->delRecord(0, chart->numRecords());
}
*/

} // namespace UI

#endif /* GENUI_CHART_H */
