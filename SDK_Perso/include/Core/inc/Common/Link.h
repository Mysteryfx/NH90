//-------------------------------------------------------------
#ifndef Link_H
#define Link_H
//-------------------------------------------------------------

#include "Common.h"
#include "Serializer/Serializer.h"

class LinkHost;

// double-linked list node
struct LinkBaseNode {
    LinkBaseNode *prev;
    LinkBaseNode *next;

        LinkBaseNode() { reset(); }
        void reset() { prev = this; next = this; }

    // list owner-/member-ship does not transfer
    LinkBaseNode(const LinkBaseNode&) { reset(); }
    LinkBaseNode& operator = (const LinkBaseNode&) { return *this; }
};

class LinkBase : LinkBaseNode
{
  friend class LinkHost;
  protected:
    LinkHost *host;
    ED_CORE_EXTERN  void Set(LinkHost *theHost);
  public:
    ED_CORE_EXTERN  LinkBase(void);
    ED_CORE_EXTERN  ~LinkBase(void);

    ED_CORE_EXTERN  LinkBase(const LinkBase&);
    ED_CORE_EXTERN  LinkBase &operator =(const LinkBase& source);

    inline bool operator ==(const LinkBase &source) const {return host==source.host;}
    inline bool operator !=(const LinkBase &source) const {return host!=source.host;}
	inline bool operator <(const LinkBase &source) const {return host<source.host;}
	inline bool operator >(const LinkBase &source) const {return host>source.host;}
};

template <class Actual = LinkHost>
  class Link : public LinkBase
  {
    public:
      Link(void) {}
	  Link(const Link<Actual>& source) {Set(source);}
      Link(Actual *theHost) {Set(theHost);}

      inline operator Actual *(void) const {return static_cast<Actual *>(host);}
      inline Actual *operator ->(void) const {return static_cast<Actual *>(host);}
      Link &operator =(Actual *theHost) {Set(theHost); return *this;}
	  
	  Actual &operator [](int i) const {return static_cast<Actual *>(host)[i];}
  };

class LinkHost 
{
  friend class LinkBase;
  private:
    LinkBaseNode store; // classic double-linked ring

  protected:
    LinkHost(void) {}

    ~LinkHost(void) { ResetLinks(); }

    // ptr ownership does not transfer
    LinkHost(const LinkHost&) {}
    LinkHost& operator = (const LinkHost&) { return *this; }

  public:
    ED_CORE_EXTERN  void ResetLinks(void);
};

template <class T>
Serializer& operator <<(Serializer& serializer, Link<T>& link)
{
	T *p = link;
	serializer << p;
	if(serializer.isLoading()) link = p;
	return serializer;
}

#endif // Link_H
