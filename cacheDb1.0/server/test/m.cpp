#include <string.h>
#include <iostream>
#include <sparsehash/dense_hash_map>
using namespace __gnu_cxx ;
using google::dense_hash_map;      // namespace where class lives by default
using std::cout;
using std::endl;
using namespace std ;

int strcmp(char s1, char s2) {
    return s1>s2?1:0 ;
}

struct eqstr
{
  bool operator()(const char s1, const char s2) const
  {
    return (s1 == s2) || (s1 && s2 && strcmp(s1, s2) == 0);
  }
};

int main()
{
  dense_hash_map<const char, int,hash<const char>, eqstr> months;

  months.set_empty_key(NULL);
  months["january"] = 31;
  months["february"] = 28;
  months["march"] = 31;
  months["april"] = 30;
  months["may"] = 31;
  months["june"] = 30;
  months["july"] = 31;
  months["august"] = 31;
  months["september"] = 30;
  months["october"] = 31;
  months["november"] = 30;
  months["december"] = 31;

  cout << "september -> " << months["september"] << endl;
  cout << "april     -> " << months["april"] << endl;
  cout << "june      -> " << months["june"] << endl;
  cout << "november  -> " << months["november"] << endl;
}

