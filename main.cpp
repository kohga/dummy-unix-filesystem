#include "filsys.h" 
#include <stdio.h> 
#include <memory> 
#include <iostream>


void split (std::vector<std::string> &v, const std::string &input_string,const std::string &delimiter){
	std::string::size_type index = input_string.find_first_of(delimiter);
	if (index != std::string::npos) {
		v.push_back( input_string.substr(0, index));
		split(v, input_string.substr(index + 1), delimiter);
	} else {
		v.push_back( input_string );
	}
}

int main( int argc , char* argv[] ) {
	std::shared_ptr<BlockDevice> bd = std::make_shared<BlockDevice>(FILE_NAME);
	std::unique_ptr<FilSys> fs = FilSys::parse(bd);

	std::shared_ptr<std::vector<std::shared_ptr<Inode> > > nodes;
	nodes = Inode::parse (bd , fs->s_isize );
	fprintf(stderr, "node size = %d\n", ( int )nodes->size());
	for (int i = 0; i < nodes->size() ; ++i) {
		std::shared_ptr<Inode> node = nodes->at( i ) ;
		if (node->isDirectory) {
			node->createIndex(bd);
		}
	}
	uint32_t nid = 1;
	std::vector<std::string> v;

	while (true) {
		std::string cmd;
		printf(">>" );
		getline(std::cin, cmd); // read 1 line
		split(v, cmd, " ");
		if (v.at(0) == "ls") {
			std::shared_ptr<Inode> node = nodes->at(nid);
			int size = v.size();

			if (size == 1 && node->isDirectory ){
				printf("%s", node->getChildren ().c_str ()); //OK
			} else if (size == 2 && node->isDirectory) {
				if (v.at (1) == "-l") {
					printf("%s", node->getChildrenDetail(nodes).c_str ());
				} else {
					printf ("Is option : -l\n" );
				}
			}
		} else if (v.at(0) == "cd") {
			std::shared_ptr<Inode> node = nodes->at(nid);
			if (node->isDirectory) {
				nid = node->getNodeld(v.at(1));
			}
		} else if (v.at(0) == "exit") {
			break;
		} else {
			printf("unknown command\n" ) ;
		}
		v.clear();
		v.shrink_to_fit ();
	}
	return 0;
}
