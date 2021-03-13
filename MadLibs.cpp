// Copyright (c) 2021 Justin Lanfermann

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>


using namespace std;

const string ARCADE = ".\\lib\\madlibs_arcade.txt";
const string SCHOOL = ".\\lib\\madlibs_school.txt";
const string FUN_PARK = ".\\lib\\madlibs_fun_park.txt";
const string JUNGLE = ".\\lib\\madlibs_jungle.txt";
const string LIBRARY = ".\\lib\\madlibs_library.txt";
const string VIDEO_GAME = ".\\lib\\madlibs_video_game.txt";
const string ZOO = ".\\lib\\madlibs_zoo.txt";
const string BIG_MAC = ".\\lib\\madlibs_big_mac.txt";
const string TOY = ".\\lib\\madlibs_toy.txt";
const string DISNEY_WORLD = ".\\lib\\madlibs_disney_world.txt";

const vector<string> SCENARIO_LIST{ARCADE, SCHOOL, FUN_PARK, JUNGLE, LIBRARY, VIDEO_GAME, ZOO, BIG_MAC, TOY, DISNEY_WORLD};

int madlibs()
{
	printf("Pick a scenario: \n1) At the Arcade!		2) The First Day of School		3) The Fun Park!\n"
		   "4) In The Jungle!		5) At the Library			6) Make me a Video Game!\n" 
		   "7) A Day at the Zoo!		8) Big Mac Who?				9) The Great New Toy!\n"
		   "10) My Trip to Disney World!\n"); 

	int scenario;
	cin >> scenario;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	
	vector<string> my_list = {};
	string word;

	if (scenario - 1 < SCENARIO_LIST.size())
	{
		ifstream myfile(SCENARIO_LIST[scenario - 1]);

		if (myfile.is_open())
		{
			while (myfile >> word)
			{
				my_list.push_back(word);
			}
			myfile.close();

			vector<string> prompt_vector;
			vector<tuple<string, int, int>> replace_list;

			for (int i = 0; i < my_list.size(); i++)
			{
				string prompt;
				prompt = my_list[i];

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
								if (my_list[i + z][my_list[i + z].length()] == '>' || my_list[i + z][my_list[i + z].length() - 1] == '>')
								{
									if (z == 1)
									{
										prompt.append(" " + my_list[i + z]);
									}
									else
									{
										prompt.append(my_list[i + z]);
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
										prompt.append(" " + my_list[i + z] + " ");
									}
									else
									{
										prompt.append(my_list[i + z] + " ");
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
					my_list[get<1>(replace_list[i])] = get<0>(replace_list[i]);
				}
			}
			int index_shift = 0;

			for (int i = 0; i < replace_list.size(); i++)
			{
				if (get<2>(replace_list[i]) > 0)
				{
					int start_point = get<1>(replace_list[i]) - index_shift;
					int add_to_start = get<2>(replace_list[i]);

					my_list.erase(my_list.begin() + start_point, my_list.begin() + start_point + add_to_start + 1);
					my_list.insert(my_list.begin() + start_point, get<0>(replace_list[i]));

					index_shift += add_to_start;
				}
			}
			const vector<string> SPECIAL_CHARS = { ".", ",", "!", "?", "'", "@", "\"" };
			string full_text;

			for (int i = 0; i < my_list.size(); i++)
			{
				string current_word = my_list[i];
				if (i + 1 < my_list.size())
				{
					if (find(SPECIAL_CHARS.begin(), SPECIAL_CHARS.end(), my_list[i + 1]) != SPECIAL_CHARS.end())
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
				const string save_path = (".\\Saved\\" + SCENARIO_LIST[scenario - 1].substr(0, (SCENARIO_LIST[scenario - 1].size())));

				ofstream outfile;

				outfile.open(save_path, ios::app);
				outfile << full_text << "\n\n\n\n";
				outfile.close();
				cout << "Saved\n";
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
		cout << "Invalid input" << endl;
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
