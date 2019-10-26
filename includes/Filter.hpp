#ifndef FILTER_HPP
# define FILTER_HPP

# include <string>
# include <math.h>
enum e_filter_type {FILTER_SLIDING_MEAN, FILTER_EXP_SMOTH, FILTER_ADAPT_SMOTH};
#define MEAN_TYPE_PREV 0
#define MEAN_TYPE_FUTURE 1

class Filter
{
private:
	float	*data;				// raw signal;
	size_t	dataSize;				// size of data
	float	*filteredData;		// procesed data
	size_t	filteredDataSize;
	// bool	isSlidingMean;		// 
	// bool	isExpSmothing;

public:
	float	*getData() const;
	size_t	getDataSize() const;
	float	*getFilteredData() const;

	void	setData(float *data);
	void	setDataSize(size_t DataSize);
	void	setFilteredData(float *p_filteredData);
	void	filterSlidingMean(size_t windowWidth, int meanType);
	void	filterExpSmooth(float alpha);
	void	filterAdaptSmoth(const size_t windowWidth, float noisePower);
	inline bool	isOverSmoothed(float raw, float filtered, float noisePower);
	void	smoothWindowWidth(int *windowWidthArray, size_t size);
	void	prepareFilteredData();
	Filter();
	Filter(Filter const &cpy);
	Filter &operator=(Filter const &cpy);
	Filter(float *p_data, size_t p_DataSize);
	~Filter();
};




#endif
