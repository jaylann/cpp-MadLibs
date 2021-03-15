// MadLibs.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <filesystem>

using namespace std;


int madlibs()
{

	string path = ".\\lib\\";
	
	vector<string> scenario_list;
	vector<string> name_list;
	vector<string> save_string;

	for (const auto& entry : filesystem::directory_iterator(path))
	{	
		string path_string{ entry.path().string() };

		int it = path_string.find("_");

		scenario_list.push_back(path_string);

		path_string.erase(0, it+1);
		path_string[0] = toupper(path_string[0]);

		save_string.push_back(path_string);

		for (int i = 0; char& c : path_string)
		{
			if (c == '_')
			{
				path_string[i+1] = toupper(path_string[i+1]);
				path_string[i] = ' ';
			}
			i++;
		}
		path_string.erase(path_string.size() - 4, 4); 

		if (path_string.size() < 27)
		{
			int padding = 27 - path_string.size();

			for (int i = 0; i < padding; ++i)
			{
				path_string.append(" ");
			}
		}
		if (path_string.size() > 27)
		{
			int rem_padding = path_string.size() - 24;

			for (int i = 0; i < rem_padding; i++)
			{
				path_string.pop_back();
			}
			path_string.append("...");
		}
		name_list.push_back(path_string);
	}
	cout << "Pick a Scenario:" << endl;

	for (int i = 0; string scenario : scenario_list)
	{
		if ((i+1) % 3 == 0)
		{
			printf("%d) %s		\n", i+1, name_list[i].c_str());
		}
		else
		{
			printf("%d) %s		", i+1, name_list[i].c_str());
		}
		i++;
	}
	cout << endl;

	int scenario;
	cin >> scenario;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	
	vector<string> word_list = {};
	string word;

	if (scenario - 1 < scenario_list.size())
	{
		ifstream in_file(scenario_list[scenario - 1]);

		if (in_file.is_open())
		{
			while (in_file >> word)
			{
				word_list.push_back(word);
			}
			in_file.close();

			vector<string> prompt_vector;
			vector<tuple<string, int, int>> replace_list;

			for (int i = 0; i < word_list.size(); i++)
			{
				string prompt;
				prompt = word_list[i];

				if (prompt.length() > 2)
				{
					if (prompt[0] == '<' || prompt[1] == '<')
					{
						if (prompt[prompt.length()] == '>' || prompt[prompt.length() - 1] == '>')
						{
							prompt.erase(remove(prompt.begin(), prompt.end(), '<'), prompt.end());
							prompt.erase(remove(prompt.begin(), prompt.end(), '>'), prompt.end());
							prompt_vector.push_back(prompt);

							cout << prompt << ": \n";

							string answer;
							getline(cin, answer);

							tuple<string, int, int> replace_tuple = { answer, i,0 };
							replace_list.push_back(replace_tuple);
						}
						else
						{
							bool prompt_open = true;
							int z = 1;

							while (prompt_open)
							{
								if (word_list[i + z][word_list[i + z].length()] == '>' || word_list[i + z][word_list[i + z].length() - 1] == '>')
								{
									if (z == 1)
									{
										prompt.append(" " + word_list[i + z]);
									}
									else
									{
										prompt.append(word_list[i + z]);
									}
									prompt.erase(remove(prompt.begin(), prompt.end(), '<'), prompt.end());
									prompt.erase(remove(prompt.begin(), prompt.end(), '>'), prompt.end());
									prompt_vector.push_back(prompt);

									cout << prompt << ": \n";

									string answer;
									getline(cin, answer);

									tuple<string, int, int> replace_tuple = { answer, i,z };
									replace_list.push_back(replace_tuple);

									prompt_open = false;
								}
								else
								{
									if (z == 1)
									{
										prompt.append(" " + word_list[i + z] + " ");
									}
									else
									{
										prompt.append(word_list[i + z] + " ");
									}
									z++;
								}
							}
						}
					}
				}
			}
			for (int i = 0; i < replace_list.size(); i++)
			{
				if (get<2>(replace_list[i]) == 0)
				{
					word_list[get<1>(replace_list[i])] = get<0>(replace_list[i]);
				}
			}
			int index_shift = 0;

			for (int i = 0; i < replace_list.size(); i++)
			{
				if (get<2>(replace_list[i]) > 0)
				{
					int start_point = get<1>(replace_list[i]) - index_shift;
					int add_to_start = get<2>(replace_list[i]);

					word_list.erase(word_list.begin() + start_point, word_list.begin() + start_point + add_to_start + 1);
					word_list.insert(word_list.begin() + start_point, get<0>(replace_list[i]));

					index_shift += add_to_start;
				}
			}
			const vector<string> SPECIAL_CHARS = { ".", ",", "!", "?", "'", "@", "\"" };
			string full_text;

			for (int i = 0; i < word_list.size(); i++)
			{
				string current_word = word_list[i];
				if (i + 1 < word_list.size())
				{
					if (find(SPECIAL_CHARS.begin(), SPECIAL_CHARS.end(), word_list[i + 1]) != SPECIAL_CHARS.end())
					{
						cout << current_word;
						full_text.append(current_word);
					}
					else
					{
						cout << current_word + " ";
						full_text.append(current_word + " ");
					}
				}
				else
				{
					cout << current_word;
					full_text.append(current_word);
				}
			}
			cout << "\n\n(n) Would you like to save the text? (y/n)\n";

			string write_to_file;
			getline(cin, write_to_file);

			for (char& ch : write_to_file)
			{
				ch = tolower(ch);
			}
			if (write_to_file[0] == 'y')
			{
				if (!filesystem::exists(".\\saved\\"))
				{
					filesystem::create_directory(".\\saved\\");
				}
				const string save_path = (".\\saved\\" + save_string[scenario - 1].substr(0, (save_string[scenario - 1].size())));

				ofstream outfile;

				outfile.open(save_path, ios::app);
				outfile << full_text << "\n\n\n\n";
				outfile.close();
				cout << "\nSaved\n";
			}
			cout << "\n\n(n) Would you like to play again? (y/n)\n";

			string repeat_game;
			getline(cin, repeat_game);

			for (char& ch : repeat_game)
			{
				ch = tolower(ch);
			}
			if (repeat_game[0] == 'y')
			{
				cout << "\n\n\n\n\n\n\n\n";
				return 1;
			}
			else
			{
				cout << "Exiting...";
				return 0;
			}
		}
		else
		{
			cout << "Unable to open file\n\n";
			return 1;
		}
	}
	else
	{
		cout << "Invalid input\n" << endl;
		return 1;
	}
	
}

int main()
{
	bool playing = true;
	while (playing == true)
	{
		playing = madlibs();
	}
	exit(0);
}
