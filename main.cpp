#include <iostream>
#include <string>
#include <fstream>

using namespace std;

bool bigBoy(int i, string input1, int semiCo); //function returns whether the corresponding secondary conditions is true

bool hclTester(string input1, int semiCo);

bool isNumber(char a);

bool pidTester(string input1, int semiCo); //tests whether the pid code is correct

bool hgtValid(string input1, int semiCo);

bool yearRobust(string input1, int semiCo, int min, int max);

int main()
{
  ifstream inputObject1("input.txt");
  string input1;

  int valid=0; //keeps track of valid entries
  int valid2=0; //keeps track of valid entries if entries are valid
  int semiCo=0; //location of semicolons
  bool isValid=0;
  bool isValid2=0;
  bool cond[7] {};
  bool cond2[7] {};
  string codes[7] {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};

  while(getline(inputObject1, input1))
  {
    if(input1.empty()) //if empty skip this line
    {
      for(int i=0; i<7; ++i) //resets all the conditions
      {
        cond[i]=false;
        cond2[i]=false;
      }
      semiCo=0;            //resets next item to 0
    }
    else //should find what conditions are there
    {
      do
      {
        semiCo=input1.find(":",semiCo+1); //sets the first semiCo location
        for(int i=0; i<7; ++i)            //sets all matching conditions true for the line
        {
          if(input1.substr(semiCo-3,3)==codes[i])
          {
            cond[i]=true;
            cond2[i]=bigBoy(i, input1, semiCo);   //sees if secondary conditions are true
          }
        }
      } while(input1.find(":", semiCo+1)!=-1); //Exits after last semicolon in line   
    }
    semiCo=0; //reset semiCo

    //After we have gone through all lines check if they are all valid
    isValid=true;
    isValid2=true;
    for(int i=0; i<7; ++i)
    {
      if(cond[i]==false)
      {
        isValid = false;
      }
      if(cond2[i]==false)
      {
        isValid2 = false;
      }
    }
    if(isValid)
    {
      ++valid;
      for(int i=0; i<7; ++i) //resets all the conditions
      {                      //THIS IS NECESSARY IF LAST LINE ONLY CONTAINS "cid" VALID TWICE ISSUE
        cond[i]=false;
      }
    }

    if(isValid2)
    {
      ++valid2;
      for(int i=0; i<7; ++i)
      {
        cond2[i]=false;
      }
    }
  }

  cout << valid << endl;
  cout << valid2 << endl;

  return 0;
}

bool bigBoy(int i, string input1, int semiCo)
{
  int bYear{};
  int iYear{};
  int eYear{};
  int hgt{};
  int endPoint{};
  string unit{};
  bool hclValid{true};
  bool tempValid{false};
  bool bigBoyValid{false};
  int unitPlace{};
  string nine{};


  switch (i)
  {
    case 0: //byr - SHOULD BE GOOD AND ROBUST
      return yearRobust(input1, semiCo, 1920, 2002);
      break;

    case 1: //iyr - SHOULD BE GOOD AND ROBUST
      return yearRobust(input1, semiCo, 2010, 2020);
      break;

    case 2: //eyr - SHOULD BE GOOD AND ROBUST
      return yearRobust(input1, semiCo, 2020, 2030);
      break;

    case 3: //hgt - THIS SHOULD BE GOOD AN ROBUST
      return hgtValid(input1, semiCo);
      break;

    case 4: //hcl
      return hclTester(input1, semiCo);
      break;

    case 5: //ecl - THIS IS GOOD AND ROBUST
      if(input1.find(" ", semiCo)==-1)
      {
        if((input1.size()-semiCo-1)>3)
        {
          return false;
        }
      }
      else if((input1.find(" ", semiCo)-semiCo-1)>3)
      {
        return false;
      }

      if(input1.substr(semiCo+1,3)=="amb"){
        return true;
      }
      if(input1.substr(semiCo+1,3)=="blu"){
        return true;
      }
      if(input1.substr(semiCo+1,3)=="brn"){
        return true;
      }
      if(input1.substr(semiCo+1,3)=="gry"){
        return true;
      }
      if(input1.substr(semiCo+1,3)=="grn"){
        return true;
      }
      if(input1.substr(semiCo+1,3)=="hzl"){
        return true;
      }
      if(input1.substr(semiCo+1,3)=="oth"){
        return true;
      }
      else{
        return false;
      }
      break;

    case 6: //THIS IS GOOD AND ROBUST
      return pidTester(input1, semiCo);
      break;

    default:
      return false;
  }

}

bool hclTester(string input1, int semiCo) //I think this is good (NEED TO CHECK THIS)
{
  bool charValid[6] {};
  bool hclValid{true};

  if(input1[semiCo+1]!='#')
  {
    return false;
  }

  if(input1.find(" ", semiCo)==-1) //these statements make sure string is correct size
  {
    if((input1.size()-semiCo-1) != 7)
    {
      return false;
    }
  }
  else if((input1.find(" ", semiCo)-semiCo-1) != 7)
  {
    return false;
  }

  for(int i=0; i<6; ++i)
  {
    for(int j=0; j<10; ++j)
    {
      if(input1[semiCo+2+i]==(48+j)) //if the character is equal to any valid number, it is valid
      {
        charValid[i]=true;
      }
    }
    for(int j=0; j<6; ++j)
    {
      if(input1[semiCo+2+i]==(97+j)) //If the character is equal to any valid character, it is valid
      {
        charValid[i]=true;
      }
    }
  }

  for(int i=0; i<6; ++i)
  {
    if(charValid[i]==false)
    {
      hclValid=false;
    }
  }

  return hclValid;
}

bool isNumber(char a)
{
  for(int i=48; i<58; ++i)
  {
    if(a==i)
    {
      return true;
    }
  }
  return false;
}

bool pidTester(string input1, int semiCo)
{
  string dataString{};
  int nextSpace{}; //next space location

  if(input1.find(" ", semiCo)==-1)
  {
    dataString=input1.substr(semiCo+1,input1.size()-semiCo-1);
  }
  else
  {
    nextSpace=input1.find(" ", semiCo);
    dataString=input1.substr(semiCo+1, nextSpace-semiCo-1);
  }

  //dataString now holds the data we will be analyzing
  if(dataString.size()!=9)
  {
    return false;
  }

  for(int i=0; i<9; ++i)
  {
    if(!isNumber(dataString[i]))
    {
      return false;
    }
  }

  return true;
  
}

bool hgtValid(string input1, int semiCo)
{
  string dataString{};
  int nextSpace{}; //next space location

  if(input1.find(" ", semiCo)==-1)
  {
    dataString=input1.substr(semiCo+1,input1.size()-semiCo-1);
  }
  else
  {
    nextSpace=input1.find(" ", semiCo);
    dataString=input1.substr(semiCo+1, nextSpace-semiCo-1);
  }

  //dataString now holds the data we will be analyzing
  bool stringValid{true}; //string starts off at valid
  string unit{};
  string s_number{}; //the number as a string
  int i_number{}; //the number as an int

  unit=dataString.substr(dataString.size()-2, 2); //unit of height is stored in this string

  if(unit != "cm" && unit != "in") //tests for the correct unit in the correct location
  {
    stringValid=false;
    return false;
  }

  for(int i=0; i<dataString.size()-2; ++i) //for loops checks that characters before unit are numbers
  {
    if(!isNumber(dataString[i]))
    {
      stringValid=false;
      return false;
    }
  }

  s_number=dataString.substr(0, dataString.size()-2);
  i_number=stoi(s_number);

  if(unit == "cm")
  {
    if(i_number < 150 || i_number > 193)
    {
      stringValid=false;
    }
  }
  else if(unit == "in")
  {
    if(i_number < 59 || i_number > 76)
    {
      stringValid=false;
    }
  }

  return stringValid;
}

bool yearRobust(string input1, int semiCo, int min, int max)
{
  string dataString{};
  int nextSpace{}; //next space location

  if(input1.find(" ", semiCo)==-1)
  {
    dataString=input1.substr(semiCo+1,input1.size()-semiCo-1);
  }
  else
  {
    nextSpace=input1.find(" ", semiCo);
    dataString=input1.substr(semiCo+1, nextSpace-semiCo-1);
  }

  //dataSTring now holds the data we will be analyzing
  for(int i=0; i<dataString.size(); ++i)
  {
    if(!isNumber(dataString[i]))
    {
      return false;
    }
  }

  int yr{stoi(dataString)};
  if(yr>=min && yr<=max)
  {
    return true;
  }
  else
  {
    return false;
  }
}