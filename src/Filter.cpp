#include "Filter.hpp"
#include <iostream>

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





void	Filter::filterAdaptSmoth(const size_t windowWidth, float noisePower)
{
	float	lambda = 0;
	float	tmp = 0;
	int		*windowWidthArray = NULL;
	prepareFilteredData();


	if (data && filteredData)
	{
		windowWidthArray = new int[dataSize]();
		filteredData[0] = data[0];
		for (size_t i = windowWidth; i < dataSize - windowWidth; i++)
		{
			filteredData[i] = 0;
			size_t w = windowWidth;
			while (w >= 0)
			{
				lambda = 1 / (2 * w + 1);
				for (size_t w = -windowWidth; w < windowWidth; w++)
				{
					filteredData[i] += data[i + w];
				}
				filteredData[i] /= lambda;
				if (isOverSmoothed(data[i], filteredData[i], noisePower))
					 w--;
				else
				{
					// std::cout << "w: " << w << std::endl;
					windowWidthArray[i] = w;
					break;
				}
			}
		}
		for (size_t i = 0; i < dataSize - 1; i++)
			if (windowWidthArray[i + 1] > windowWidthArray[i])
				windowWidthArray[i + 1] = windowWidthArray[i] + 1;
		for (size_t i = dataSize - 1; i > 0; i--)
			if (windowWidthArray[i] < windowWidthArray[i - 1])
				windowWidthArray[i - 1] = windowWidthArray[i] + 1;

		for (size_t i = windowWidth; i < dataSize - windowWidth; i++)
		{
			filteredData[i] = 0;
			lambda = 1 / (2 * windowWidthArray[i] + 1);
			for (size_t w = -windowWidthArray[i]; w < windowWidthArray[i]; i++)
			{
				if ( i == 100 )
					std::cout << "W: " << w << "i: " << i << std::endl;
				filteredData[i] += data[i + w];
				
			}
			filteredData[i] /= lambda;
			
		}
		delete []windowWidthArray;
	}

	// float	lambda = 0;
	// float	tmp = 0;
	// int		*windowWidthArray = NULL;
	// prepareFilteredData();


	// if (data && filteredData)
	// {
	// 	windowWidthArray = new int[dataSize];
	// 	filteredData[0] = data[0];
	// 	for (size_t i = windowWidth + 1; i < dataSize - windowWidth; i++)
	// 	{
	// 		size_t w = windowWidth;
	// 		while (w >= 0)
	// 		{
	// 			lambda = 1 / (2 * w + 1);
	// 			filteredData[i] = filteredData[i - 1] + lambda * (data[i + windowWidth] - data[i - windowWidth - 1]);
	// 			if (isOverSmoothed(data[i], filteredData[i], noisePower))
	// 				 w--;
	// 			else
	// 			{
	// 				windowWidthArray[i] = w;
	// 				break;
	// 			}
	// 		}
	// 	}
	// 	for (size_t i = 0; i < dataSize - 1; i++)
	// 		if (windowWidthArray[i + 1] > windowWidthArray[i])
	// 			windowWidthArray[i + 1] = windowWidthArray[i] + 1;
	// 	for (size_t i = dataSize - 1; i > 0; i--)
	// 		if (windowWidthArray[i] < windowWidthArray[i - 1])
	// 			windowWidthArray[i - 1] = windowWidthArray[i] + 1;

	// 	for (size_t i = 1; i < dataSize - windowWidth; i++)
	// 	{
	// 		lambda = 1 / (2 * windowWidthArray[i] + 1);
	// 		filteredData[i] = filteredData[i - 1] + lambda * (data[i + windowWidthArray[i]] - data[i - windowWidthArray[i] - 1]);
	// 	}
	// 	delete []windowWidthArray;
	// }

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

