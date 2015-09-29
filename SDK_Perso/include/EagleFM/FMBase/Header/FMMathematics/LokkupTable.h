#pragma once

namespace EagleFM {

class LookupTableUtilities
{
public:

	/*Use this function when elements count less than 18*/
	template <typename T,typename ForwardIterator>
	static T linearinterpolate(ForwardIterator x_b, ForwardIterator x_e, 
		ForwardIterator y_b, ForwardIterator y_e, const T value)
	{
		auto it = x_b;

		while(it != x_e)
		{
			if (value <= *it) break;
			++it;
		}

		auto n = it - x_b;

		if(n == 0) return *(y_b);

		if((y_e - y_b) <= n) return *(y_e-1);

		T x2 = *it;
		--it;

		y_b += n;

		return (*y_b*(value - *it) + *(y_b - 1)*(x2-value))/(x2-*it);
	}

	template <typename Matrix, typename ResultsVector>
	static void linearinterpolateMatrix(Matrix matrix, ResultsVector& results)
	{
		auto x_b  = matrix[0].begin();
		auto x_e  = matrix[0].end();

		auto it = x_b;
		auto value = results[0];

		while(it != x_e)
		{
			if (value <= *it) break;
			++it;
		}

		auto n = it - x_b;

		if(n == 0 || n == matrix[0].size())
		{
			for(size_t i = 1; i < results.size(); ++i)
				results[i] = matrix[i][n-1];

			return;
		}

		auto x2 = *it;
		--it;

		for(size_t i = 1; i < results.size(); ++i)
			results[i] =  (matrix[i][n]*(value - *it) + matrix[i][n-1]*(x2-value))/(x2-*it);
	}

	/*Binary search version. use this function when elements count more than 18*/
	template <typename T,typename ForwardIterator>
	static T linearinterpolate_binary(ForwardIterator x_b, ForwardIterator x_e, 
		ForwardIterator y_b, ForwardIterator y_e, const T value)
	{
		auto it = std::lower_bound(x_b,x_e,value);

		auto n = it - x_b;

		if(n == 0) return *(y_b);

		if((y_e - y_b) <= n) return *(y_e-1);

		T x2 = *it;
		--it;

		y_b += n;

		return (*y_b*(value - *it) + *(y_b - 1)*(x2-value))/(x2-*it);
	}
};
}