#ifndef __CLUSTERISATOR__
#define __CLUSTERISATOR__

#include "common_Export.h"

#include <ed/list.h>
#include <ed/vector.h>
#include <utility>
#include <ed/set.h>
#include <ed/map.h>
#include "math.h"
#include "math/Vector.h"

namespace Common {

enum MedObjectDistType	{ OBJ_EUCLID_DIST, OBJ_EUCLID_DIST_SQR, OBJ_EUCLID_MANHATTAN, OBJ_MAX};	
enum MedClusterDistType	{ CLS_SINGLE_LINKAGE, COMPLETE_LINKAGE, CLS_UPGMA, CLS_WPGMA, CLS_UPGMC, CLS_WPGMC, CLS_MAX };
typedef double			Coord;
typedef Coord			Dist;

template<typename T, unsigned int N>
class Clusterisator
{
public:	
	typedef Math::Vector<N, Coord>	Coords;	
	typedef ed::list<T>			Cluster;
	typedef ed::list<Cluster>		Clusters;
	typedef Coords (*PGetCoord)(const T & t);
	class Terminator
	{
	public:
		virtual bool operator()(const Clusterisator<T,N> & clusterisator) const = 0;
	};
	class TerminatorDistLimit : public Terminator
	{
	public:
		TerminatorDistLimit(Dist maxDist_) : maxDist(maxDist_) {;}
		bool operator()(const Clusterisator<T,N> & clusterisator) const;
		Dist maxDist;
	};
	class TerminatorClustersQtyLimit : public Terminator
	{
	public:
		TerminatorClustersQtyLimit(unsigned int maxClusterQty_) : maxClusterQty(maxClusterQty_) {;}
		bool operator()(const Clusterisator<T,N> & clusterisator) const;
		unsigned int maxClusterQty;
	};
	class ObjectChecker
	{
	public:
		virtual bool operator()(const T & left, const T & right) const = 0;
	};
	friend class Terminator;
private:
	typedef std::pair<typename Clusters::iterator, typename Clusters::iterator> ClusterPair;
	struct ClusterPairData
	{
		ClusterPairData() : Ujk(0.0f) {;}
		bool operator<(const ClusterPairData & clusterPairData) const
		{ 
			assert(Ujk != clusterPairData.Ujk);
			return Ujk < clusterPairData.Ujk;
		}
		ClusterPair clusterPair;
		Dist		Ujk;
	};
	typedef ed::set<ClusterPairData> ClusterPairsData;
	struct PClusterPair
	{
		std::pair<Cluster*, Cluster*> pCPair_;
		PClusterPair(Cluster* first_, Cluster* second_)
		{
			assert(first_ != second_);
			if(first_ > second_)
			{
				pCPair_.first = first_;
				pCPair_.second = second_;
			}
			else
			{
				pCPair_.first = second_;
				pCPair_.second = first_;
			}
		}
		bool operator < (const PClusterPair & pClusterPair) const
		{
			return pCPair_ < pClusterPair.pCPair_;
		}
	};
	typedef ed::map<PClusterPair, typename ClusterPairsData::iterator>  Links;
public:
	//functor
	Clusterisator();
	Clusterisator(	MedClusterDistType medClusterDistType_, MedObjectDistType medObjectDistType_, 
					Terminator * terminator_, ObjectChecker * pObjectChecker, PGetCoord pGetCoord_);
	bool operator()(const ed::vector<T> & tvec, Clusters & clusters);
	//calculator
	void reset();
	void addObject(const T & t, Clusters & clusters);
	void start(const ed::vector<T> & tvec, Clusters & clusters);
	bool nextStep(unsigned int stepQty, Clusters & clusters);
private:
	void mergeClusters(typename ClusterPairsData::iterator itCPD, Clusters & clusters);
private:
	static Dist calcDist(const Coords & coords1, const Coords & coords2, MedObjectDistType medObjectDistType);
	static Dist calcFusedClusterDist(const ClusterPairData & clusterPairData, Dist Ujl, Dist Ukl, MedClusterDistType medClusterDistType);
private:
	MedClusterDistType	medClusterDistType;
	MedObjectDistType	medObjectDistType;
	Terminator *		terminator;
	ObjectChecker *		pObjectChecker;
	PGetCoord			pGetCoord;
	ClusterPairsData	clusterPairsData;
	Links				links;

};

//-=Clusterisator<T, N>::TerminatorDistLimit=-

template<typename T, unsigned int N>
inline bool Clusterisator<T, N>::TerminatorDistLimit::operator()(const Clusterisator<T, N> & clusterisator) const
{
	return	clusterisator.clusterPairsData.empty() ||
			clusterisator.clusterPairsData.begin()->Ujk > maxDist;
}

//-=Clusterisator<T, N>::TerminatorClustersQtyLimit=-

template<typename T, unsigned int N>
inline bool Clusterisator<T, N>::TerminatorClustersQtyLimit::operator()(const Clusterisator<T, N> & clusterisator) const
{
	return clusterisator.clusters.size() < maxClusterQty;
}

//-=Clusterisator<T, N>=-

template<typename T, unsigned int N>
inline Clusterisator<T, N>::Clusterisator() : medClusterDistType(0), medObjectDistType(0), terminator(NULL), pObjectChecker(NULL), pGetCoord(NULL)
{

}

template<typename T, unsigned int N>
inline Clusterisator<T, N>::Clusterisator(MedClusterDistType medClusterDistType_, MedObjectDistType	medObjectDistType_, 
										  Terminator * terminator_, ObjectChecker * pObjectChecker_, PGetCoord pGetCoord_)
{
	medClusterDistType	= medClusterDistType_; 
	medObjectDistType	= medObjectDistType_;
	terminator			= terminator_;
	pObjectChecker		= pObjectChecker_;
	pGetCoord			= pGetCoord_;
}

template<typename T, unsigned int N>
inline bool Clusterisator<T, N>::operator()(const ed::vector<T> & tvec, Clusters & clusters)
{
	clusterPairsData.clear();
	links.clear();

	for(ed::vector<T>::const_iterator itT = tvec.begin();
		itT != tvec.end();
		itT++)
	{
		const T & t = (*itT);
		Clusters::iterator itCnew = clusters.insert(clusters.end(), Cluster());
		itCnew->push_back(t);

		for(Clusters::iterator itC = clusters.begin();
			itC != clusters.end();
			itC++)
		{
			if(itC != itCnew)
			{
				if(	pObjectChecker == NULL ||
					(*pObjectChecker)(t, itC->front()))
				{
					ClusterPairData clusterPairData;
					clusterPairData.clusterPair.first = itCnew;
					clusterPairData.clusterPair.second = itC;
					clusterPairData.Ujk = calcDist( (*pGetCoord)(itCnew->front()), (*pGetCoord)(itC->front()), medObjectDistType);
					links[PClusterPair(&*itCnew, &*itC)] = clusterPairsData.insert(clusterPairData).first;
				}
			}
		}
	}

	while(	clusters.size() > 1 &&
			!(*terminator)(*this))
	{
		mergeClusters(clusterPairsData.begin(), clusters);
	}
	return !clusters.empty();
}

template<typename T, unsigned int N>
inline void Clusterisator<T, N>::addObject(const T & t, Clusters & clusters)
{
	const T & t = (*itT);
	Clusters::iterator itCnew = clusters.insert(clusters.end(), Cluster());
	itCnew->push_back(t);

	for(Clusters::iterator itC = clusters.begin();
		itC != clusters.end();
		itC++)
	{
		if(itC != itCnew)
		{
			ClusterPairData clusterPairData;
			clusterPairData.clusterPair.first = itCnew;
			clusterPairData.clusterPair.second = itC;
			clusterPairData.Ujk = calcDist( (*pGetCoord)(itCnew->front()), (*pGetCoord)(itC->front()), medObjectDistType );				
			links[PClusterPair(&*itCnew, &*itC)] = clusterPairsData.insert(clusterPairData).first;
		}
	}
}

template<typename T, unsigned int N>
inline void Clusterisator<T, N>::reset()
{
	clusterPairsData.clear();
	links.clear();
}

template<typename T, unsigned int N>
inline void Clusterisator<T, N>::start(const ed::vector<T> & tvec, Clusters & clusters)
{
	reset();
	for(ed::vector<T>::const_iterator itT = tvec.begin();
		itT != tvec.end();
		itT++)
	{
		const T & t = (*itT);
		Clusters::iterator itCnew = clusters.insert(clusters.end(), Cluster());
		itCnew->push_back(t);

		for(Clusters::iterator itC = clusters.begin();
			itC != clusters.end();
			itC++)
		{
			if(itC != itCnew)
			{
				ClusterPairData clusterPairData;
				clusterPairData.clusterPair.first = itCnew;
				clusterPairData.clusterPair.second = itC;
				clusterPairData.Ujk = calcDist( (*pGetCoord)(itCnew->front()), (*pGetCoord)(itC->front()), medObjectDistType );				
				links[PClusterPair(&*itCnew, &*itC)] = clusterPairsData.insert(clusterPairData).first;
			}
		}
	}	
}

template<typename T, unsigned int N>
inline bool Clusterisator<T, N>::nextStep(unsigned int stepQty, Clusters & clusters)
{
	unsigned int stepCounter = 0;
	bool finish = false;
	while(	!(finish = (clusters.size() < 2 || (*terminator)(*this))) &&
			stepCounter < stepQty)
	{
		
		stepCounter++;
		mergeClusters(clusterPairsData.begin(), clusters);
	}
	if(finish)
	{
		reset();
	}
	return !finish;
}

template<typename T, unsigned int N>
inline void Clusterisator<T, N>::mergeClusters(typename ClusterPairsData::iterator itCPD, Clusters & clusters)
{
	const ClusterPairData & mergingClusterPairData = (*itCPD);	
	Links::iterator itP = links.find(PClusterPair(&*mergingClusterPairData.clusterPair.first, &*mergingClusterPairData.clusterPair.second));
	assert(itP != links.end());
	links.erase(itP);

	unsigned int i = 0;
	for(Clusters::iterator itC = clusters.begin();
		itC != clusters.end();
		itC++)
	{
		if(	itC != mergingClusterPairData.clusterPair.first &&
			itC != mergingClusterPairData.clusterPair.second)
		{
			i++;
			Links::iterator itF = links.find(PClusterPair(&*itC, &*mergingClusterPairData.clusterPair.first));
			assert(itF != links.end());
			Links::iterator itS = links.find(PClusterPair(&*itC, &*mergingClusterPairData.clusterPair.second));
			assert(itS != links.end());
			if(itF != links.end() || itS != links.end())
			{
				ClusterPairData fusedClusterPairData;
				fusedClusterPairData.clusterPair.first = mergingClusterPairData.clusterPair.first;
				fusedClusterPairData.clusterPair.second = itC;
				Dist Ujk_l = calcFusedClusterDist(mergingClusterPairData, itF->second->Ujk, itS->second->Ujk, medClusterDistType);
				fusedClusterPairData.Ujk = Ujk_l;
				clusterPairsData.erase(itF->second);
				clusterPairsData.erase(itS->second);
				itF->second = clusterPairsData.insert(fusedClusterPairData).first;
				links.erase(itS);
			}
		}
	}	
	mergingClusterPairData.clusterPair.first->merge(*mergingClusterPairData.clusterPair.second);
	clusters.erase(mergingClusterPairData.clusterPair.second);	
	clusterPairsData.erase(itCPD);
}

template<typename T, unsigned int N>
inline Dist Clusterisator<T, N>::calcDist(const Coords & coords1, const Coords & coords2, MedObjectDistType medObjectDistType)
{
	switch(medObjectDistType)
	{
		case OBJ_EUCLID_DIST:
			return length(coords2 - coords1);
		case OBJ_EUCLID_DIST_SQR:
			return length2(coords2 - coords1);
		case OBJ_EUCLID_MANHATTAN:
			{
				Dist dist = 0.0f;
				for(unsigned int i = 0; i < N; i++)
				{
					dist += fabs(coords2[i] - coords1[i]);
				}
				return dist;
			}			
		default:
			return 0.0f;
	}
}

template<typename T, unsigned int N>
inline Dist Clusterisator<T, N>::calcFusedClusterDist(const ClusterPairData & clusterPairData, Dist Ujl, Dist Ukl, MedClusterDistType medClusterDistType)
{
	switch(medClusterDistType)
	{
		case CLS_SINGLE_LINKAGE:
			return 0.5f * (Ujl + Ukl - fabs(Ujl - Ukl));
		case COMPLETE_LINKAGE:
			return 0.5f * (Ujl + Ukl + fabs(Ujl - Ukl));
		case CLS_UPGMA:
			{
				size_t Nj = clusterPairData.clusterPair.first->size();
				size_t Nk = clusterPairData.clusterPair.second->size();
				size_t Njk = Nj + Nk;
				return (Nj * Ujl + Nk * Ukl)/ Njk;
			}
		case CLS_WPGMA:
			{
				size_t Nj = clusterPairData.clusterPair.first->size();
				size_t Nk = clusterPairData.clusterPair.second->size();
				size_t Njk = Nj + Nk;
				return (Nj * Ujl + Nk * Ukl - Nj * Nk * clusterPairData.Ujk  / Njk ) / Njk;
			}			
		case CLS_UPGMC:
			return 0.5f * (Ujl + Ukl);
		case CLS_WPGMC:
			return 0.5f * (Ujl + Ukl - 0.5f * clusterPairData.Ujk);
		default:
			return 0.0f;
	}
}

} // namespace Common

#endif
