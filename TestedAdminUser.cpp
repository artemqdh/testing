#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <string.h>


void Display_Questions(std::string questions_folder) // Displays questions from folder in files
{
	for (const auto& file : std::filesystem::directory_iterator(questions_folder))
	{
		std::ifstream file1(file.path());
		if (!file1.is_open())
		{
			std::cout << "Error. Could not open file: " << file.path() << std::endl;
			continue;
		}

		std::string line;
		int num = 0;
		while (getline(file1, line))
		{
			std::cout << line << std::endl;
			++num;
			if (!(num&1)) // endl if num is 1,3,5,7,9 and no endl if 2,4,6,8
			{
				std::cout << std::endl;
			}
		}
	}
}

void StartTest(std::string answers_folder) // start test + statistic in %
{
	int right_answers = 0;
	int wrong_answers = 0;
	int total_questions = 0;

	for (const auto& file : std::filesystem::directory_iterator(answers_folder))
	{
		std::ifstream file1(file.path());
		if (!file1.is_open())
		{
			std::cout << "Error. Could not open file: " << file.path() << std::endl;
			continue;
		}
		std::string line;
		std::string answer1;
		while (getline(file1, line))
		{
			++total_questions;
			std::cout << "Your answer: ";
			std::getline(std::cin >> std::ws, answer1);
			if (line == answer1)
			{
				++right_answers;
				std::cout << "Correct!" << std::endl;
			}
			else
			{
				++wrong_answers;
				std::cout << "Wrong!" << std::endl;
			}
		}
	}

	double percentage = 0.0;
	if (total_questions > 0)
	{
		percentage = (static_cast<double>(right_answers) / total_questions) * 100;
	}
	if (percentage > 60)
	{
		std::cout << "You have passed the test with " << percentage << " percent right answers!" << std::endl;
	}
	else
	{
		std::cout << "You have not passed the test, because you have " << percentage << " percent right answers, but you need over 60%!" << std::endl;
	}
}

class Test
{
public:
	Test(std::string name_file_test_question1, std::string name_file_test_answer1,std::string correct_answer1, std::string category1)
	{
		name_file_test_question = name_file_test_question1;
		name_file_test_answer = name_file_test_answer1;
		correct_answer = correct_answer1;
		category = category1;
	}

private:
	std::string name_file_test_question;
	std::string name_file_test_answer;
	std::string category;
	std::string correct_answer;
	std::string name_statistics_test;
};

std::map <std::string, std::vector<Test>> category_and_all_test;

class User
{
public:
	virtual void Registration() = 0;
	virtual bool Check_login(std::string login1) = 0;
	virtual bool Check_pass(std::string pass1) = 0;
};

class Admin : public User
{
public:
	void Registration() override
	{
		bool isEmptyFile = !std::filesystem::exists(name_login_pass_file) || std::filesystem::is_empty(name_login_pass_file);
		std::ofstream file1(name_login_pass_file, std::ios::app);
		
		if (file1.is_open())
		{
			if (isEmptyFile)
			{
				std::string login1, pass1;
				std::cout << "Login and password: " << std::endl;
				std::cin >> login1 >> pass1;

				file1 << GetCrypt(login1) << " " << GetCrypt(pass1) << std::endl;
				file1.close();
			}
		}
	}
	
	bool Check()
	{
		std::cout << "Login and password: ";
		std::string login1;
		std::string pass1;
		std::cin >> login1 >> pass1;
		if (Check_login(login1) && Check_pass(pass1))
		{
			std::cout << "Welcome " << login1 << "!" << std::endl;
			return true;
		}
		else
		{
			std::cout << "Wrong login or password." << std::endl;
			return false;
		}
	}

	bool Check_login(std::string login1) override
	{
		std::ifstream file1(name_login_pass_file);
		std::string line;
		std::string a;
		while (getline(file1, line))
		{
			a = line;
		}

		std::string b;
		for (int i = 0; i < a.size(); ++i)
		{
			if (a[i] != ' ')
			{
				b += a[i];
			}
			else if (a[i] == ' ')
			{
				a = b;
			}
		}
		std::string word = Encryption(a);

		if (login1 == word)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool Check_pass(std::string pass1) override
	{
		std::ifstream file1(name_login_pass_file);
		std::string line;
		std::string a;
		while (getline(file1, line))
		{
			a = line;
		}

		std::string b;
		for (int i = 0; i < a.size(); ++i)
		{
			if (a[i] == ' ')
			{
				++i;
				b = a.substr(i);
			}
		}
		std::string word = Encryption(b);

		if (pass1 == word)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	std::string Encryption(std::string word)
	{
		std::string a;
		for (int i = 0; i < word.size(); ++i)
		{
			a += word[i] - 1;
		}
		return a;
	}

	void Add_Test() 
	{
		std::cout << "Enter the new category: ";
		std::string new_category;
		std::getline(std::cin >> std::ws, new_category);

		std::filesystem::create_directories(new_category);
		std::filesystem::create_directories(new_category + "/questions");	// all folders for example mathematics -> answers and questions
		std::filesystem::create_directories(new_category + "/answers");

		std::cout << "How many questions do you want to add?" << std::endl;
		int num_questions = 0;
		std::cin >> num_questions;

		int question_number = 1;

		for (int i = 0; i < num_questions; ++i) 
		{
			std::cout << "Enter the new question: ";
			std::string new_question;
			std::getline(std::cin >> std::ws, new_question);

			std::cout << "Enter the new answers (separated by -): ";
			std::string new_answers;
			std::getline(std::cin >> std::ws, new_answers);

			std::cout << "Enter the correct answer: ";
			std::string correct_answer;
			std::getline(std::cin >> std::ws, correct_answer);

			std::string question_filename = new_category + "/questions/question" + std::to_string(question_number) + ".txt";	// for example
			std::string answer_filename = new_category + "/answers/answer" + std::to_string(question_number) + ".txt";			// mathematics/questions/question1.txt

			std::ofstream question_file(question_filename);
			std::ofstream answer_file(answer_filename);

			if (question_file.is_open() && answer_file.is_open()) 
			{
				question_file << new_question << std::endl;
				question_file << new_answers << std::endl;
				answer_file << correct_answer << std::endl;
				std::cout << "Question and answer was successfully added." << std::endl;
			}
			else 
			{
				std::cout << "Error. File is not open." << std::endl;
			}
			++question_number; // question1 -> question2 // answer1 -> answer2
		}
	}

	void Delete_Test()
	{
		std::cout << "Enter the category name to delete: ";
		std::string category_name;
		std::getline(std::cin >> std::ws, category_name);
		if (std::filesystem::exists(category_name) && std::filesystem::is_directory(category_name))	// check if folder exists
		{
			std::filesystem::remove_all(category_name);
			std::cout << category_name << " was successfully deleted." << std::endl;
		}
		else
		{
			std::cout << "Error. " << category_name << " does not exist." << std::endl;
		}
	}

	void Delete_Tested() 
	{
		std::cout << "Who do you want to delete? (name_family)" << std::endl;
		std::string name1;
		std::cin >> name1;

		std::string name_file = "all_users/" + name1 + ".txt";
		std::string login_file = "all_users/login_" + name1 + ".txt";

		if (std::filesystem::exists(name_file)) 
		{
			std::filesystem::remove(name_file);
			std::cout << name1 << " was successfully deleted." << std::endl;	// for example: deleted artem_galimov.txt
		}
		else 
		{
			std::cout << "Error. " << name1 << " does not exist." << std::endl;
		}

		if (std::filesystem::exists(login_file)) 
		{
			std::filesystem::remove(login_file);
			std::cout << "login_" << name1 << " was successfully deleted." << std::endl;	// for example: deleted login_artem_galimov.txt
		}
		else {
			std::cout << "Error. " << name1 << " does not exist." << std::endl;
		}
	}

private:
	std::string GetCrypt(std::string a)
	{
		std::string a_login;
		for (int i = 0; i < a.size(); i++)
		{
			a_login.push_back(a[i] + 1);
		}
		return a_login;
	}
	std::string name_login_pass_file = "admin_login_pass.txt";
	std::string name_login_pass_file_tested = "tested_login_pass";
	std::string name_test = "new_test";
};

class Tested : public User
{
public:
	friend class Admin;
	Tested(std::string name_person_data_file1, std::string name_statistics_file1, std::string name_login_pass_file1, std::string name_file_test_incomplete1)
	{
		name_person_data_file = name_person_data_file1;
		name_statistics_file = name_statistics_file1;
		name_login_pass_file = name_login_pass_file1;
		name_file_test_incomplete = name_file_test_incomplete1;
	}

	void Registration() override
	{
		std::string name1;
		std::string adress1;
		std::int32_t phonenum1;
		std::cout << "Name (name_family), adress and phonenum(maximum 9 digits): " << std::endl;
		std::cin >> name1 >> adress1 >> phonenum1;

		std::ofstream file2("all_users/" + name1 + ".txt", std::ios::app); // all_users = folder // example = all_users/artem_galimov.txt
		if (file2.is_open())
		{
			file2 << name1 << " " << adress1 << " " << phonenum1;
			file2.close();
		}
		
		std::ofstream file1("all_users/login_" + name1 + ".txt", std::ios::app); // all_users = folder // example = all_users/login_artem_galimov.txt
		if (file1.is_open())
		{
			std::string login1;
			std::string pass1;
			std::cout << "Create login and password: " << std::endl;
			std::cin >> login1 >> pass1;

			file1 << GetCrypt(login1) << " " << GetCrypt(pass1) << std::endl;
			file1.close();
		}
		else
		{
			std::cout << "File not open!" << std::endl;
		}
		std::cout << "Success! Now you can do tests." << std::endl;
	}

	bool Check()
	{
		std::cout << "Name(name_family): ";
		std::string name1;
		std::cin >> name1;
		name = name1;

		std::ifstream file1("all_users/login_" + name1 + ".txt");
		if (file1.is_open())
		{
			std::cout << "Login and password: ";
			std::string login1;
			std::string pass1;
			std::cin >> login1 >> pass1;

			if (Check_login(login1) && Check_pass(pass1))
			{
				std::cout << "Welcome " << login1 << "!" << std::endl;
				return true;
			}
			else
			{
				std::cout << "Wrong login or password." << std::endl;
				return false;
			}
		}
		name.clear();
	}

	bool Check_login(std::string login1) override
	{
		std::ifstream file1("all_users/login_" + name + ".txt");
		if (file1.is_open())
		{
			std::string line;
			std::string a;
			while (getline(file1, line))
			{
				a = line;
			}


			std::string b;
			for (int i = 0; i < a.size(); ++i)
			{
				if (a[i] != ' ')
				{
					b += a[i];
				}
				else if (a[i] == ' ')
				{
					a = b;
				}
			}

			std::string word = Encryption(a);
			if (login1 == word)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	bool Check_pass(std::string pass1) override
	{
		std::ifstream file1("all_users/login_" + name + ".txt");
		if (file1.is_open())
		{
			std::string line;
			std::string a;
			while (getline(file1, line))
			{
				a = line;
			}

			std::string b;
			for (int i = 0; i < a.size(); ++i)
			{
				if (a[i] == ' ')
				{
					++i;
					b = a.substr(i);
				}
			}
			std::string word = Encryption(b);
			if (pass1 == word)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	std::string Encryption(std::string word)
	{
		std::string a;
		for (int i = 0; i < word.size(); ++i)
		{
			a += word[i] - 1;
		}
		return a;
	}

private:
	std::string GetCrypt(std::string a)
	{
		std::string a_login;
		for (int i = 0; i < a.size(); i++)
		{
			a_login.push_back(a[i] + 1);
		}
		return a_login;
	}
	std::string name;
	std::string name_person_data_file;
	std::string name_statistics_file;
	std::string name_login_pass_file;
	std::string name_file_test_incomplete;
};

std::vector <Tested> anyone_tested;

int main()
{
	Admin b;
	Tested a("a.txt", "b.txt", "c.txt", "d.txt");
	
	std::cout << "Welcome" << std::endl;

	std::cout << "Where do you want to login? 1 - Admin, 2 - Tested" << std::endl;
	int num1 = 0;
	std::cin >> num1;

	switch (num1)
	{
	case 1:
	{
		b.Registration();
		b.Check();

		std::cout << "1 - Add Test\n2 - Delete Test\n3 - Delete Tested" << std::endl;
		int num5 = 0;
		std::cin >> num5;
		
		switch (num5)
		{
			case 1:
			{
				b.Add_Test();
				break;
			}
			case 2:
			{
				b.Delete_Test();
				break;
			}
			case 3:
			{
				b.Delete_Tested();
				break;
			}
		}
		break;
	}
	case 2:
	{
		std::cout << "Do you want to 1 - registrate or 2 - login?" << std::endl;
		int num2 = 0;
		std::cin >> num2;

		switch (num2)
		{
		case 1:
		{
			a.Registration();
			break;
		}
		case 2:
		{
			if (a.Check())
			{
				std::cout << "What test do you want to start?" << std::endl;
				std::cout << "1 - basic_knowledge, 2 - mathematics, 3 - football and 4 - biology" << std::endl;
				int num = 0;
				std::cin >> num;
				system("cls");
				switch (num)
				{
					case 1:
					{
						Display_Questions("basic_knowledge/questions");
						StartTest("basic_knowledge/answers");
						break;
					}
					case 2:
					{
						Display_Questions("mathematics/questions");
						StartTest("mathematics/answers");
						break;
					}
					case 3:
					{
						Display_Questions("football/questions");
						StartTest("football/answers");
						break;
					}
					case 4:
					{
						Display_Questions("biology/questions");
						StartTest("biology/answers");
						break;
					}
				}
			}
		}
		}
	}
	}
}
