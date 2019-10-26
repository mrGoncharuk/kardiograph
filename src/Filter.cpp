#include "Filter.hpp"
#include <iostream>

Filter::Filter()
	: data(NULL)
	, dataSize(0)
	, filteredData(NULL)
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
		for (size_t i = 0; i < dataSize; i++)
			data[i] = cpy.data[i];
	}
	filteredDataSize = cpy.filteredDataSize;
	if (cpy.filteredData && filteredDataSize > 0)
	{
		data = new float[filteredDataSize];
		for (size_t i = 0; i < filteredDataSize; i++)
			data[i] = cpy.filteredData[i];
	}
	return (*this);
}


Filter::~Filter()
{
	if (filteredData)
	{
		delete []filteredData;
		filteredData = NULL;
		dataSize = 0;
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


void	Filter::filterSlidingMean(size_t windowWidth, int type)
{
	float	lambda;

	prepareFilteredData();
	if (data && filteredData)
	{
		filteredData[0] = data[0];

		if (type == MEAN_TYPE_PREV && windowWidth)
		{
			lambda = 1.0f / windowWidth;
			for (size_t i = windowWidth; i < dataSize; i++)
				filteredData[i] = filteredData[i - 1] + lambda * (data[i] - data[i - windowWidth]);
		}
		if (type == MEAN_TYPE_FUTURE)
		{
			lambda = 1.0f / (1 + 2 * windowWidth);
			for (size_t i = windowWidth; i < dataSize - windowWidth; i++)
				filteredData[i] = filteredData[i - 1] + lambda * (data[i + windowWidth] - data[i - windowWidth]);
		}
	}
}

bool	Filter::isOverSmoothed(float raw, float filtered, float noisePower)
{
	return (fabs(filtered - raw) > noisePower);
}

void	Filter::smoothWindowWidth(int *windowWidthArray, size_t size)
{
	for (size_t i = 0; i < size - 1; i++)
		if (windowWidthArray[i + 1] > windowWidthArray[i])
			windowWidthArray[i + 1] = windowWidthArray[i] + 1;
	for (size_t i = size - 1; i > 0; i--)
		if (windowWidthArray[i] < windowWidthArray[i - 1])
			windowWidthArray[i - 1] = windowWidthArray[i] + 1;

}


void	Filter::filterAdaptSmoth(const size_t windowWidth, float noisePower)
{
	float	lambda = 0;
	int		*windowWidthArray = NULL;
	prepareFilteredData();

	if (data && filteredData)
	{
		windowWidthArray = new int[dataSize]();
		filteredData[0] = data[0];
		for (size_t i = windowWidth; i < dataSize - windowWidth; i++)
		{
			filteredData[i] = 0;
			windowWidthArray[i] = 0;
			int w = windowWidth;
			while (w > 0)
			{
				lambda = 1.0f / (2 * w + 1);
				for (int j = -w; j <= w; j++)
				{
					filteredData[i] += data[i + j];
				}
				filteredData[i] *= lambda;
				if (isOverSmoothed(data[i], filteredData[i], noisePower))
					 w--;
				else
				{
					windowWidthArray[i] = w;
					break;
				}
			}
		}
		smoothWindowWidth(windowWidthArray, dataSize);
		for (size_t i = windowWidth; i < dataSize - windowWidth; i++)
		{
			filteredData[i] = 0;
			lambda = 1.0f / (2 * windowWidthArray[i] + 1);
			for (int w = -windowWidthArray[i]; w <= windowWidthArray[i]; w++)
				filteredData[i] += data[i + w];
			filteredData[i] *= lambda;
			
		}
		delete []windowWidthArray;
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
	{
		delete []filteredData;
		filteredData = p_filteredData;
	}
}

