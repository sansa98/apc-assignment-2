// DataFrame.cpp source file

#include <iomanip>
#include "DataFrame.h"

std::string DataFrame::PREFIX1{"df1_"};
std::string DataFrame::PREFIX2{"df2_"};


// Helper functions Protos:
void tabbed(double value, bool showZeros = true);
void tabbed(std::string string);
void tab();
std::vector<std::string> addPrefix(const std::vector<std::string> & to, std::string prefix);




DataFrame::DataFrame(const std::string &columnNamesString) : DataFrame() {
    Keys columnsNames = split(columnNamesString, ' ');

    for (const Key &name: columnsNames)
        dataFrameData[name];
}


DataFrame::Keys DataFrame::split(const std::string &s, char delim) const {
    Key word;
    Keys v;
    std::istringstream columns(s);

    while (std::getline(columns, word, delim)) v.push_back(word);

    return v;
}

bool DataFrame::checkColumnName(const std::string &columnName) const {
    return dataFrameData.find(columnName) != dataFrameData.end();
}


Column DataFrame::getColumn(const std::string &columnName) const {
    if (checkColumnName(columnName))
        return dataFrameData.at(columnName);
    else {
        std::cerr << "Error, " << columnName << " is unknown" << std::endl;
        return Column();
    }
}


void DataFrame::setColumn(const std::string &columnName,
                           const Column &column) {
    if (checkColumnName(columnName))
        dataFrameData[columnName] = column;
    else
        std::cerr << "Error, " << columnName << " is unknown" << std::endl;
}



bool DataFrame::addColumn(const Key &columnName,
                           const Column &columnData) {
    if (!checkColumnName(columnName)) {
        dataFrameData[columnName] = columnData;
        return true;
    }
    else {
        return false;
    }
}



std::vector<DataFrame::Key> DataFrame::getColumnNames() const {

    Keys names = Keys();

    for (const valueType &element: dataFrameData) {
        std::string s = element.first;
        names.push_back(s);
    }

    return names;
}


// // we se are simulating arrays, number or rows will be the max value in keys
dimension DataFrame::getDimension() const {
    auto colNames = getColumnNames();
    if (colNames.empty()){
        return {0,0};
    }

    auto cols = colNames.size();

    // cannot be equal, se we have sparse columns:

    /* if dense, every column with same height, so:
    auto values0 = get_column(colNames[0]);
    auto rows = values0.height();
     */
    
    // but we must search for max key:
    size_t rows = 0;
    for(std::string name: colNames){
        auto values = getColumn(name);
        auto H = values.height();
        rows = std::max(rows, H);
    }
    
    return {rows, cols};
}



void DataFrame::print(void) const {

    // write all columns names:
    auto colNames = getColumnNames();
    for (auto t: colNames) {
        tabbed(t);
    }

    newLine();

    auto dimension = getDimension();
    
    if (dimension.rows == 0)
        return;

    for (int row = 0; row < dimension.rows; row++) {

        auto valuesArray = getValuesAtRow(row);
            for(auto v : valuesArray){
                tabbed(v);
            }
            newLine();
    }

    newLine();

}








// TO BE IMPLEMENTED:

DataFrame
DataFrame::hstack(DataFrame &otherDataFrame){
    DataFrame DataFrame;

    // copy first table in empty DataFrame
    /*for (const valueType &element: dataFrameData) {
        std::string s = element.first;
        Column a = getColumn(element.first);
        DataFrame.addColumn(PREFIX1+s,a);
    }*/

    // copy first table in empty DataFrame
    Keys columnNamesFirst = getColumnNames();
    for (auto t : columnNamesFirst){
        Column newColumn = getColumn(t);
        DataFrame.addColumn(PREFIX1+t, newColumn);
    }

    //attach the second table to first table
    Keys columnNamesSecond = otherDataFrame.getColumnNames();
    for (auto t : columnNamesSecond){
        Column newColumn = otherDataFrame.getColumn(t);
        DataFrame.addColumn(PREFIX2+t, newColumn);
    }

    return DataFrame;
}

DataFrame DataFrame::join(DataFrame &otherDataFrame, std::string onMyCol, std::string onColOfOther){
    bool a = false;

    std::map<size_t,ValuesArray> new1;
    std::vector<ValuesArray> new2;

    int numberRows = 0;

    Column firstColumn = getColumn(onMyCol);
    Column secondColumn = otherDataFrame.getColumn(onColOfOther);

    for (size_t i = 0; i<firstColumn.values.size(); ++i){
        for (size_t j = 0; j<secondColumn.values.size() ; ++j){
            if (firstColumn.values[i] == secondColumn.values[j]){   //elemento i-esimo nella colonna del PRIMO
                new1[numberRows] = getValuesAtRow(i);
                new2.push_back(otherDataFrame.getValuesAtRow(j));
                numberRows++;
                a = true;
            }
        }
    }

    Keys columnNamesFirst = getColumnNames();
    Keys columnNamesSecond = otherDataFrame.getColumnNames();


    if (a == false){
        DataFrame dataFrame1(columnNamesFirst);
        DataFrame dataFrame2(columnNamesSecond);
        return dataFrame1.hstack(dataFrame2);
    }

// PERCHE NON USO ITERATORS
    DataFrame dataFrame1;
    for (size_t j = 0; j<new1.at(0).size(); j++){  //new1.at(0) = numberCols
        Column col;
        for (size_t i = 0; i<new1.size(); i++){   //new1.size() = numberRows
            col.insert(new1[i][j],i);
        }
        dataFrame1.addColumn(columnNamesFirst[j],col);
    }

    DataFrame dataFrame2;
    for (size_t j = 0; j<new2.at(0).size(); j++){   //new2.at(0) = numberCols
        Column col;
        for (size_t i = 0; i<new2.size(); i++){    //new2.size() = numberRows
            col.insert(new2[i][j],i);
        }
        dataFrame2.addColumn(columnNamesSecond[j],col);
    }

    return dataFrame1.hstack(dataFrame2);
}






// ancillary code

DataFrame::DataFrame(std::vector<std::string> columnsNames)
: DataFrame() {
    for (const Key &name: columnsNames)
        dataFrameData[name];
}


ValuesArray DataFrame::getValuesAtRow(size_t row) const{
    ValuesArray result = {};
    
    auto colNames = getColumnNames();
    for (auto name:colNames) {
        auto column = getColumn(name);
        auto value = column.values[row];
        result.push_back(value);
    }
    return result;
}


// MARK: Helper functions:



void tabbed(double value, bool showZeros){

    if (value == 0 && !showZeros){
        std::cout << "       " << "|";
    }else{
        std::cout << std::setw(7);
        std::cout << value << "|";
    }
}



void tabbed(std::string string){
    auto stdLen = 7;
    auto len = string.size();
    auto spaces = stdLen-len;
    std::cout << string;
    for (int i = 0; i < spaces; ++i) {
        std::cout << " ";
    }
    std::cout << "|";

}

void tab(){
    tabbed("");
}

void newLine(std::string s){
    std::cout << s << std::endl;
}



std::vector<std::string> addPrefix(const std::vector<std::string> & to, std::string prefix){
    std::vector<std::string> out;

    std::transform(to.begin(), to.end(),
                   std::back_inserter(out),
                   [&prefix](std::string v) { return prefix+v; }
    );
    return out;
}

