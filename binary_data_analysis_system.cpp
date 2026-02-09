#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

const int SIZE = 1000;

/**
 * Writes an integer array into a binary file
 * @param array pointer to integer array
 * @param length the length of the array
 */
void writeBinary(int* values, int length, const string& name)
{
    // open binary file for output
    ofstream out_file;
    out_file.open("binary.dat", ios::out | ios::binary);

    // check for failure
    if (out_file.fail())
    {
        cout << "Unable to open binary file to write." << endl;
        return;
    }

    // write integer that represents array's length
    out_file.write(reinterpret_cast<const char*>(&length), sizeof(int));

    // write remaining integers in array into binary file
    out_file.write(reinterpret_cast<const char*>(values), sizeof(int) * length);

    // close binary file
    out_file.close();
}

/**
 * Creates a binary file with array of 1000 random integers
 * @param name name of file
 * @param length size of array
 */
void createBinaryFile(const string& name, int length)
{
    // create dynamic array of integers
    int* array = new int[length];

    // fill array with random integers from 0 to 999
    for (int i = 0; i < length; i++)
    {
        array[i] = rand() % 1000;
    }

    writeBinary(array, length, name);
    delete[] array;
    array = nullptr;
}

/**
 * Gets position of smallest element in array
 * @param values pointer to array of integers
 * @param from beginning of range
 * @param to end of range
 * @return position of smallest element
 */
int min_position(int* values, int from, int to)
{
    int min_pos = from;
    for (int i = from + 1; i <= to; i++)
    {
        if (values[i] < values[min_pos])
        {
            min_pos = i;
        }
    }
    return min_pos;
}

/**
 * Swaps two integers
 * @param x the first integer
 * @param y the second integer
 */
void swap(int& x, int& y)
{
    int temp = x;
    x = y;
    y = temp;
}

/**
 * Sorts an array using selection sort
 * @param values pointer to array of integers
 * @param size the size of array
 */
void selection_sort(int* values, int size)
{
    int next;

    for (next = 0; next < size - 1; next++)
    {
        int min_pos = min_position(values, next, size - 1);
        swap(values[next], values[min_pos]);
    }
}

/**
 * Recursively searches for value in array
 * @param values pointer to array of integers
 * @param key the value we are searching for
 * @param start the starting index
 * @param end the ending index
 * @return Boolean value representing if key is found
 */
bool binary_search_recursive(int* values, int key, int start, int end)
{
    // Base case
    if (start > end) { return false; }

    int mid = (start + end) / 2;

    // Return true if mid is value we are looking for
    if (key == values[mid]) { return true; }

    // If value is smaller than mid, look in first half
    else if (key < values[mid]) { return binary_search_recursive(values, key, start, mid - 1); }

    // If value is bigger than mid, look in second half
    else { return binary_search_recursive(values, key, mid + 1, end); }
}

/**
 * Helper function for binary_search_recursive
 * @param values pointer to array of integers
 * @param size the size of array
 * @param key the value we are searching for
 * @return Boolean value returned by recursive search function
 */
bool binary_search(int* values, int size, int key)
{
    return binary_search_recursive(values, key, 0, size - 1);
}

class BinaryReader
{
public: 
    /**
     * Constructs binary reader
     * @param name the binary file name
     */
    BinaryReader(const string& name);

    // Destructor to delete integer array
    ~BinaryReader();

    // Getter for integer array
    int* getValues() const;

    // Getter for size
    int getSize() const;

private:
    int* values;
    int size;

    /**
     * Reads binary file for size and integer array
     * @param name the binary file name
     */
    void readValues(const string& name);
};

BinaryReader::BinaryReader(const string& name)
{
    readValues(name);
}

BinaryReader::~BinaryReader()
{
    delete[] values;
    values = nullptr;
}

int* BinaryReader::getValues() const
{
    return values;
}

int BinaryReader::getSize() const
{
    return size;
}

void BinaryReader::readValues(const string& name)
{
    // open binary file for input
    ifstream in_file;
    in_file.open("binary.dat", ios::in | ios::binary);

    // check for failure
    if (in_file.fail())
    {
        cout << "Unable to open binary file to read." << endl;
        return;
    }

    // read first value into variable that stores array length
    in_file.read(reinterpret_cast<char*>(&size), sizeof(int));

    // create dynamic integer array with length variable
    values = new int[size];

    // read each value from binary file into new array
    in_file.read(reinterpret_cast<char*>(values), sizeof(int) * size);
    in_file.close();
}

class Analyzer
{
public: 
    /**
     * Constructs an analyzer
     * @param values pointer to array of integers
     * @param size the size of array
     */
    Analyzer(int* values, int size);

    // Destructor to delete dynamic array
    ~Analyzer();

    // Pure virtual function for analyze()
    virtual string analyze() = 0;

protected: 
    int* values;
    int size;

    // Copies values and size of an array into instance variables
    void cloneValues(int* values, int size);
};

Analyzer::Analyzer(int* values, int size)
{
    cloneValues(values, size);
}

Analyzer::~Analyzer()
{
    delete[] this->values;
    this->values = nullptr;
}

void Analyzer::cloneValues(int* values, int size)
{
    this->size = size;
    this->values = new int[this->size];

    for (int i = 0; i < this->size; i++)
    {
        this->values[i] = values[i];
    }
}

class DuplicateAnalyzer : public Analyzer
{
public: 
    /**
     * Constructs an analyzer for duplicates
     * @param values pointer to array of integers
     * @param size the size of array
     */
    DuplicateAnalyzer(int* values, int size);

    // Overrides analyze() method to count duplicated values
    virtual string analyze() override;
};

DuplicateAnalyzer::DuplicateAnalyzer(int* values, int size) : Analyzer(values, size)
{
}

string DuplicateAnalyzer::analyze()
{
    int duplicate_count = 0;
    const int SIZE = 1000;
    int count[SIZE] = {0};

    // Count occurrences of each value
    for (int i = 0; i < size; i++)
    {
        count[values[i]]++;
    }

    // Count how many values are duplicates
    for (int i = 0; i < SIZE; i++)
    {
        if (count[i] > 1)
        {
            duplicate_count += 1;
        }
    }

    stringstream result;
    result << "There were " << duplicate_count << " duplicated values";
    return result.str();
}

class MissingAnalyzer : public Analyzer
{
public:
    /**
     * Constructs an analyzer for missing values
     * @param values pointer to array of integers
     * @param size the size of array
     */
    MissingAnalyzer(int* values, int size);

    // Overrides analyze() method to count missing values
    virtual string analyze() override;
};

MissingAnalyzer::MissingAnalyzer(int* values, int size) : Analyzer(values, size)
{
}

string MissingAnalyzer::analyze()
{
    // Create boolean array to track seen values
    const int SIZE = 1000;
    bool seen[SIZE] = { false };

    // Change values found in array to true
    for (int i = 0; i < size; i++)
    {
        seen[values[i]] = true;
    }

    // Count missing values
    int missing_count = 0;
    for (int i = 0; i < SIZE; i++)
    {
        if (!seen[i])
        {
            missing_count++;
        }
    }

    stringstream result;
    result << "There are " << missing_count << " missing values";
    return result.str();
}

class StatisticsAnalyzer : public Analyzer
{
public: 
    /**
     * Constructs an analyzer for statistics
     * @param values pointer to array of integers
     * @param size the size of array
     */
    StatisticsAnalyzer(int* values, int size);

    // Overrides analyze() method to perform statistical analysis
    virtual string analyze() override;
};

StatisticsAnalyzer::StatisticsAnalyzer(int* values, int size) : Analyzer(values, size)
{
    selection_sort(this->values, this->size);
}

string StatisticsAnalyzer::analyze()
{
    int min = values[0];
    int max = values[size - 1];
    double sum = 0;

    for (int i = 0; i < size; i++)
    {
        sum += values[i];
    }
    double mean = sum / size;

    double median;

    if (size % 2 == 1) { median = values[size / 2]; }

    else { median = (values[size / 2] + values[size / 2 + 1]) / 2.0; }

    int mode = values[0];
    int max_count = 1;
    int current_count = 1;

    for (int i = 1; i < size; i++)
    {
        if (values[i] == values[i - 1])
        {
            current_count++;
        }
        else 
        {
            if (current_count > max_count)
            {
                max_count = current_count;
                mode = values[i];
            }
            current_count = 1;
        }
    }


    stringstream result;
    result << "The minimum value is " << min << '\n';
    result << "The maximum value is " << max << '\n';
    result << "The mean value is " << mean << '\n';
    result << "The median value is " << median << '\n';
    result << "The mode value is " << mode << " which occurred " << max_count << " times";
    return result.str();
}

class SearchAnalyzer : public Analyzer
{
public: 
    /**
     * Constructs an analyzer for searching
     * @param values pointer to array of integers
     * @param size the size of array
     */
    SearchAnalyzer(int* values, int size);

    // Overrides analyze() method to count number of random values
    virtual string analyze() override;
};

SearchAnalyzer::SearchAnalyzer(int* values, int size) : Analyzer(values, size)
{
    selection_sort(this->values, this->size);
}

string SearchAnalyzer::analyze()
{
    int count = 0;

    for (int i = 0; i < 100; i++)
    {
        int random_value = rand() % 1000;
        bool found = binary_search(values, size, random_value);
        if (found == true) { count++; }
    }

    stringstream result;
    result << "There were " << count << " random values found";
    return result.str();
}

int main()
{
    srand(time(0));

    createBinaryFile("binary.dat", SIZE);
    BinaryReader br("binary.dat");

    StatisticsAnalyzer sa(br.getValues(), br.getSize());
    cout << sa.analyze() << '\n';
    DuplicateAnalyzer da(br.getValues(), br.getSize());
    cout << da.analyze() << '\n';
    MissingAnalyzer ma(br.getValues(), br.getSize());
    cout << ma.analyze() << '\n';
    SearchAnalyzer ra(br.getValues(), br.getSize());
    cout << ra.analyze() << '\n';

    return 0;
}