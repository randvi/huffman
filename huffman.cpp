#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <iostream>
//#include <map>
#include <fstream>
#include <vector>
#include <cstring>


using posT = std::istream::pos_type;

posT getFileSize(const std::string& fname) {
	std::ifstream ifs(fname, std::fstream::ate);
	return ifs.tellg();
}

int main() {
	const std::string file_name("advs.txt");
	
	const auto file_size = getFileSize(file_name) ;	
	if (file_size < 0) {
		std::cerr << "Error opening file <" << file_name << "> : " 
							<< std::strerror(errno) << std::endl;
		return -1;
	}
	std::cout << "Size of file <" << file_name << "> : " << file_size << std::endl;
	
	std::ifstream ifs(file_name, std::ifstream::binary);
	if (!ifs) {
		std::cerr << "Error in <" << __PRETTY_FUNCTION__ << "> : " 
							<< std::strerror(errno) << std::endl;
		return -1;
	}
	
	std::vector<int> frequency(0x100, 0);
	int read_bytes = 0;
	std::cout << "Reading file <" << file_name << "> : " <<  std::endl;
	while(!ifs.eof()) {
		char ch;
		ifs.read(&ch, 1);
		frequency[static_cast<unsigned char>(ch)]++;
		if (read_bytes % 20 == 0) {
			int value = (read_bytes + file_size % 20) * 100.0 / file_size;
			std::cout << '\r' << value << '%' << std::flush;
		} 
		++read_bytes;
	}
	std::cout << std::endl;
	
	for (size_t i = 0; i < frequency.size(); ++i) {
		std::cout << '[' << i << "] = " << frequency[i] <<  ",\t";
	}	
	std::cout << std::endl;
	
	{
		std::ofstream ofs("frequency.ser");
		boost::archive::text_oarchive oa(ofs);
		oa << frequency;
	}

	std::vector<int> frequency2;
	{
		std::ifstream ifs("frequency.ser");
		boost::archive::text_iarchive ia(ifs);
		ia >> frequency2;
	}
	
	for (size_t i = 0; i < frequency2.size(); ++i) {
		std::cout << '[' << i << "] = " << frequency2[i] <<  ",\t";
	}	
	
	std::cout << std::endl;
	
	return 0;
}
