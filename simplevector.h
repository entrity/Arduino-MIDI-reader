/***
	DataType is the class of the object stored in the vector.
	SizeType is the numerical type used for vector indices. If space is on a premium,
		use a char. If your vector will store more than 256 items, use a type larger
		than char.
*/

#ifndef ENTRITY_SIMPLEVECTOR
#define ENTRITY_SIMPLEVECTOR
#define INITIAL_ARRAY_SIZE 1

#include <stdlib.h>
#include <string.h>

template<class DataType, class SizeType>
class SimpleVector {
	public:
		SizeType length; // the length of the array (available space)
		SizeType n; // how many items are in the array
		DataType * arr; // underlying array
		SimpleVector(int initialLength = INITIAL_ARRAY_SIZE);
		~SimpleVector();
		int extend(SizeType addition); // make underlying array longer
		int push(DataType datum);
		SizeType size() { return n; }
		SizeType arrSize() { return length; }
		DataType &operator[] (SizeType index) { return arr[index]; }
};

#define ERROR_MJ 0
#define SUCCESS_MJ 1


// constructor
template<class DataType, class SizeType>
SimpleVector<DataType, SizeType>::SimpleVector(int initialLength) : n(0), length(initialLength)
{
	arr = static_cast< DataType* >( malloc( initialLength * sizeof(DataType) ) );
}

// destructor
template<class DataType, class SizeType>
SimpleVector<DataType, SizeType>::~SimpleVector()
{ 
	for (int i=0; i < n; i++)
		{ DataType toDelete = arr[i]; delete &toDelete; }
}

// extend: Make underlying array longer
template<class DataType, class SizeType>
int SimpleVector<DataType, SizeType>::extend(SizeType addition)
{
	SizeType bytesToCopy = length * sizeof(DataType);
	length += addition;
	DataType * newArr = (DataType *) malloc( length * sizeof(DataType) );
	if (newArr == NULL) { return ERROR_MJ; }
	memmove( newArr, arr, bytesToCopy);
	free(arr);
	arr = newArr;
	return SUCCESS_MJ;
}

// PUSH item onto vector
template<class DataType, class SizeType>
int SimpleVector<DataType, SizeType>::push(DataType datum)
{
	if ( n >= length )   // make longer array if necessary
	{
		if ( extend(1) == ERROR_MJ ) return ERROR_MJ;
	}
	arr[n++] = datum; // push value onto array
	return SUCCESS_MJ;
}

#undef INITIAL_ARRAY_SIZE
#endif