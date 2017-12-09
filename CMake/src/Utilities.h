#pragma once

template <typename T>
void insertionSort(std::vector<T*>& vec)
{
	int i, j;
	T* key;

	for (i = 1; i < vec.size(); i++)
	{
		key = vec[i];
		j = i - 1;

		while (j >= 0 && *vec[j] > *key)
		{
			vec[j + 1] = vec[j];
			j--;
		}

		vec[j + 1] = key;
	}
}