//
// Created by ing.conti on 13/11/22.
// TestValues.h

#ifndef DATAFRAME_TESTVALUES_H
#define DATAFRAME_TESTVALUES_H


// test case 1, as x word:
DataFrame df_1("A B C");
df_1.setColumn("A", Column("-2,1"));
df_1.setColumn("B", Column("2,3"));
df_1.setColumn("C", Column("4,5"));

DataFrame df_2("D E F");
df_2.setColumn("D", Column("6,1"));
df_2.setColumn("E", Column("7,10"));
df_2.setColumn("F", Column("8,11"));


// columns where we ask for JOIN:
std::string ON_MY_COL{"A"};
std::string ON_OTHER_COLUMN{"D"};


#endif //DATAFRAME_TESTVALUES_H
