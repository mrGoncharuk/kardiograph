#include "Filter.hpp"


Filter::Filter()
	: data(NULL)
	, filteredData(NULL)
	, dataSize(0)
	, filteredDataSize(0)
{
}

Filter::Filter(float *p_data, size_t p_DataSize)
{
	data = p_data;
	dataSize = p_DataSize;
	filteredData = NULL;
	filteredDataSize = 0;
}

Filter::Filter(Filter const &cpy)
{
	*this = cpy;
}

Filter &Filter::operator=(Filter const &cpy)
{
	if (data)
	{
		delete []data;
		data = NULL;
	}
	if (filteredData)
	{
		delete []filteredData;
		filteredData = NULL;
	}
	dataSize = cpy.dataSize;
	if (cpy.data && cpy.dataSize)
	{
		data = new float[dataSize];
		for (int i = 0; i < dataSize; i++)
			data[i] = cpy.data[i];
	}
	filteredDataSize = cpy.filteredDataSize;
	if (cpy.filteredData && filteredDataSize > 0)
	{
		data = new float[filteredDataSize];
		for (int i = 0; i < filteredDataSize; i++)
			data[i] = cpy.filteredData[i];
	}
}


Filter::~Filter()
{
	if (data)
	{
		delete []data;
		data = NULL;
		dataSize = 0;
	}
	if (filteredData)
	{
		delete []filteredData;
		filteredData = NULL;
		filteredDataSize = 0;
	}
}

void	Filter::filterExpSmooth(float alpha)
{
	prepareFilteredData();
	if (data && filteredData)
	{
		filteredData[0] = data[0];
		for (size_t i = 1; i < dataSize; i++)
			filteredData[i] = filteredData[i - 1] + alpha * (data[i] - filteredData[i - 1]);
	}
}


void	Filter::filterSlidingMean(size_t windowWidth, bool isPrev, bool isFuture)
{
	float	lambda;

	prepareFilteredData();
	if (data && filteredData)
	{
		filteredData[0] = data[0];

		if (isPrev && windowWidth)
		{
			lambda = 1.0f / windowWidth;
			for (size_t i = 1; i < dataSize; i++)
				filteredData[i] = filteredData[i - 1] + lambda * (data[i] - data[i - windowWidth]);
		}
		if (isFuture)
		{
			lambda = 1.0f / (1 + 2 * windowWidth);
			for (size_t i = windowWidth; i < dataSize - windowWidth; i++)
				filteredData[i] = filteredData[i - 1] + lambda * (data[i + windowWidth] - data[i - windowWidth]);
		}
	}
}

void	Filter::prepareFilteredData()
{
	if (data)
	{
		if ((dataSize != filteredDataSize) && filteredData)
		{
			delete []filteredData;
			filteredData = NULL;
			filteredDataSize = dataSize;
		}
		if (filteredData == NULL)
		{
			filteredDataSize = dataSize;
			filteredData = new float[filteredDataSize];
		}
	}
}

float	*Filter::getData() const { return data; }
size_t	Filter::getDataSize() const { return dataSize; }
float	*Filter::getFilteredData() const { return filteredData; }


void	Filter::setDataSize(size_t p_counts) { dataSize = p_counts; }
void	Filter::setData(float *p_data)
{
	data = p_data;
}

void	Filter::setFilteredData(float *p_filteredData)
{
	if (filteredData)
		delete []filteredData;
	filteredData = p_filteredData;
}

