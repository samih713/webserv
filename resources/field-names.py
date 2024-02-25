import csv

csv_file_path = './field-names.csv'
output_file_path = 'field-names.hpp'

first_column_values = []
with open(csv_file_path, newline='') as csvfile:
	reader = csv.reader(csvfile)
	for row in reader:
		if row:
			first_column_values.append(row[0])


# Adjusted part for C++98 static map initialization
with open(output_file_path, 'w') as hppfile:
    hppfile.write('#include <map>\n#include <string>\n\n')
    
    # Write the array of pairs
    hppfile.write('std::pair<int, std::string> init_values[] = {\n')
    for index, value in enumerate(first_column_values):
        hppfile.write(f'    std::make_pair("{value}", {index}),\n')
    hppfile.write('};\n')
    
    # Initialize the map using the array
    hppfile.write('std::map<int, std::string> myMap(init_values, init_values + sizeof(init_values) / sizeof(init_values[0]));\n')

