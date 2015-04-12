#include "stdafx.h"

#include <fstream>
#include "Conjecture.h"

void processManual()
{
	Conjecture conj;
	int cd;
	double term;
	string funName;
	ioPair_t ioPair;

	cout << "Input Function Name: ";
	cin >> funName;

	cout << "-- The input-output example of " << funName << "(n):" << endl;
	int i = 0;
	cout << funName << "(" << i << ") = ";
	while(cin >> term)
	{
		ioPair[i] = term;
		i++;
		cout << funName << "(" << i << ") = ";
	}

	ioPair_t::iterator iter;
	int begin, end;
	if (!ioPair.empty())
	{
		iter = ioPair.begin();
		begin = iter->first;
		iter = ioPair.end();
		iter--;
		end = iter->first;
	}

	Function* f1 =	conj.generalTerm(ioPair, cd);
	cout << endl << "-=-=-=- GeneralTerm Engine -=-=-=-" << endl;
	cout << "-- Confidence Degree: " << cd << " --" << endl << endl;
	if (f1)
	{
		f1->toString(funName);
		cout << "The define of " << f1->nameToString("n") << ":\n" << f1->toString() << endl << endl;
		for(int i = begin; i <= end + 8; i++)
		{
			cout << f1->nameToString(dtos(i)) << " = " << f1->funcVal(i) << endl;
		}
		delete f1;
	}
	else
	{
		cout << "No result!" << endl;
	}

	/////////////////////////////////////

	Function* f2 =	conj.synthesise(ioPair, cd);
	cout << endl << "-=-=-=- Synthesis Engine -=-=-=-" << endl;
	cout << "-- Confidence Degree: " << cd  << " --" << endl << endl;
	if (f2)
	{
		f2->toString(funName);
		cout << "The define of " << f2->nameToString("n") << ":\n" << f2->toString() << endl << endl;
		for(int i = begin; i <= end + 8; i++)
		{
			cout << f2->nameToString(dtos(i)) << " = " << f2->funcVal(i) << endl;
		}
		delete f2;
	}
	else
	{
		cout << "No result!" << endl;
	}
}

void processFile(string file)
{
	ifstream fin(file.c_str());
	if (!fin)
	{
		cout << "No such file: " << file << endl;
		return;
	}
	ofstream foutGeneral("GeneralTerm.csv");
	ofstream foutSynthesis("Synthesis.csv");
	string funName;
	string line;
	int input;
	double output;
	ioPair_t ioPair;

	Conjecture conj;
	int cd;

 	while(fin >> funName)
	{
		ioPair.clear();
		while (funName.size() <= 1)
		{
			fin	>> funName;
			if (!fin)
			{
				return;
			}
		}
		// omit the ","
		if (funName[funName.size() - 1] == ',')
		{
			funName = funName.substr(0, funName.size() - 1);
		}
		while(fin >> line)
		{
			
			if (line.size() == 1)
			{
				break;
			}
			string::size_type comma = line.find_first_of(',');
			input = stoi(line.substr(0, comma));
			output = stod(line.substr(comma + 1));
			ioPair[input] = output;
		}

		ioPair_t::iterator iter;
		int begin, end;
		if (!ioPair.empty())
		{
			iter = ioPair.begin();
			begin = iter->first;

			cout << "-- The input-output example of " << funName << "(n):" << endl;

			for(iter; iter != ioPair.end(); iter++)
			{
				cout << funName << "(" << iter->first << ") = " << iter->second << endl;
			}
			iter--;
			end = iter->first;
		}

		Function* f1 =	conj.generalTerm(ioPair, cd);
		cout << endl << "-=-=-=- GeneralTerm Engine -=-=-=-" << endl;
		cout << "-- Confidence Degree: " << cd << " --" << endl << endl;
		foutGeneral << "-- Confidence Degree: " << cd << " --" << endl << endl;
		if (f1)
		{
			f1->toString(funName);
			cout << "The define of " << f1->nameToString("n") << ":\n" << f1->toString() << endl << endl;
			foutGeneral << "The define of " << f1->nameToString("n") << ":\n" << f1->toString() << endl << endl;
			for(int i = begin; i <= end + 8; i++)
			{
				cout << f1->nameToString(dtos(i)) << " = " << f1->funcVal(i) << endl;
				foutGeneral << f1->nameToString(dtos(i)) << " = " << f1->funcVal(i) << endl;
			}
			delete f1;
		}
		else
		{
			cout << "No result!" << endl;
			foutGeneral << "No result!" << endl;
		}
		foutGeneral << endl << "===============================================================================" << endl << endl;
		
		/////////////////////////////////////
		
		Function* f2 =	conj.synthesise(ioPair, cd);
		cout << endl << "-=-=-=- Synthesis Engine -=-=-=-" << endl;
		cout << "-- Confidence Degree: " << cd  << " --" << endl << endl;
		foutSynthesis << "-- Confidence Degree: " << cd  << " --" << endl << endl;
		if (f2)
		{
			f2->toString(funName);
			cout << "The define of " << f2->nameToString("n") << ":\n" << f2->toString() << endl << endl;
			foutSynthesis << "The define of " << f2->nameToString("n") << ":\n" << f2->toString() << endl << endl;
			for(int i = begin; i <= end + 8; i++)
			{
				cout << f2->nameToString(dtos(i)) << " = " << f2->funcVal(i) << endl;
				foutSynthesis << f2->nameToString(dtos(i)) << " = " << f2->funcVal(i) << endl;
			}
			delete f2;
		}
		else
		{
			cout << "No result!" << endl;
			foutSynthesis << "No result!" << endl;
		}
		cout << endl << "===============================================================================" << endl << endl;
		foutSynthesis << endl << "===============================================================================" << endl << endl;
	}

	fin.close();
	foutGeneral.close();
	foutSynthesis.close();
}

int main(int argc, char* argv[])
{
	string file = "sample.csv";
	if (argc > 1)
	{
		file = argv[1];
		processFile(file);
	}
	else
	{
		processManual();
	}
	return 0;
}
