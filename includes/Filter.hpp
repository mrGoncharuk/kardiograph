#ifndef FILTER_HPP
# define FILTER_HPP

# include <string>

enum e_processing_type {PT_SLIDING_MEAN, PT_EXP_SMOTH, PT_ADAPT_SMOTH};

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
	void	filterSlidingMean(size_t windowWidth, bool isPrev, bool isFuture);
	void	filterExpSmooth(float alpha);
	void	filterAdaptSmoth();
	void	prepareFilteredData();
	Filter();
	Filter(Filter const &cpy);
	Filter &operator=(Filter const &cpy);
	Filter(float *p_data, size_t p_DataSize);
	~Filter();
};




#endif
