#pragma once
#include "Patient.h"
#include "Queue.h"
#include <fstream>


class PatientsHandler
{
private:
	Queue<Patient> _patients;
public:
	std::string GetStringAllPatientsInfo()
	{
		std::string AllPatientsString = "";
		Queue<Patient>::Iterator iter = _patients.begin();
		do
		{
			AllPatientsString += (*iter).GetStringPatientInfo() + "\n";
			iter++;

		} while (iter != _patients.end());

		return AllPatientsString;
	}

private:
	void LoadPatients()
	{
		std::ifstream fin;

		for (int i = 1; i < 6; i++)
		{
			std::string path = "C:\\Users\\liber\\source\\repos\\AASD_1lab_Server\\AASD_1lab_Server\\PatientsFolder\\Patient";
			path.append(std::to_string(i));
			path.append(".txt");

			fin.open(path);
			if (!fin.is_open())
				break;

			std::string TempPatientInfo;

			getline(fin, TempPatientInfo);
			fin.close();


			_patients.Enqueue(Patient(TempPatientInfo));
		}
		std::cout<< GetStringAllPatientsInfo() << std::endl;
		
	}

public:
	PatientsHandler()
		:_patients(5)
	{
		LoadPatients();
	}

	std::string GetPatient()
	{
		Patient p = _patients.Dequeue();
		return p.GetStringPatientInfo();
	}



};

