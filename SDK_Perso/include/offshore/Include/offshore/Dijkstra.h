#ifndef Dijkstra_H
#define Dijkstra_H

namespace offshore
{

template <typename Function> bool Dijkstra(int nodecount, int start, int end, ed::vector<int>& route, double maxLenght/*-1 unlimited*/, const Function& f)
{
	if( maxLenght<=0) 
		maxLenght = 1e32;
	ed::vector<float> weights(nodecount, 1e32f);
	ed::vector<int> prevpoints(nodecount, -1);

	ed::list<int> processing;
	processing.push_back(start);
	weights[start] = 0;
	while( !processing.empty())
	{
		for(auto it=processing.begin(); it!=processing.end(); it=processing.erase(it))
		{
			int v = *it;
			osg::Vec2d pt;
			ed::vector<int>& links = f(v, pt);

			for(int i=0; i<links.size(); i++)
			{
				int next = links[i];

				osg::Vec2d nextpt;
				ed::vector<int>& nextlinks = f(next, nextpt);

				float nextw = weights[v] + (pt - nextpt).length();
				if( nextw>=maxLenght)
					continue;
				if(nextw>=weights[next])
					continue;
				if(nextw>=weights[end])
					continue;
				weights[next] = nextw;
				prevpoints[next] = v;

				// add next
				processing.push_back(next);
			}
		}
	}
	if(prevpoints[end]<0)
		return false;
	route.clear();
	for(int i=end; i>=0; i=prevpoints[i])
	{
		route.push_back( i);
	}
	return true;
}


} // namespace offshore

#endif
