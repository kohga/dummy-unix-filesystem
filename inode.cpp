#include "filsys.h"

Inode::Inode() {
}

Inode::~Inode() {
}

std::unique_ptr< std::vector <std::shared_ptr <Inode> > >

Inode::parse ( std::shared_ptr <BlockDevice> bd, uint16_t size) {
	std::unique_ptr<std::vector<std::shared_ptr<Inode> > > list (new std::vector<std::shared_ptr<Inode> >); 
	bd->setIndex(2 * BlockDevice::BLOCK_SIZE);

	list->push_back(std::shared_ptr<Inode>(new Inode())); // for dummy

	for (int i = 0; i < (BlockDevice::BLOCK_SIZE * size / Inode::INODE_SIZE); ++i) {
		std::unique_ptr<Inode> node(new Inode()) ;
		node->i_mode = bd->readShort();
		node->i_nlink = bd->readChar();
		node->i_uid = bd->readChar();
		node->i_gid  = bd->readChar();
		node->i_size0 = bd->readChar(); 
		node->i_size1 = bd->readShort();

		for (int i = 0; i < ARR_SIZE(node->i_addr ); ++i ) {
			node->i_addr[i] = bd->readShort();
		}
		for (int i = 0; i < ARR_SIZE(node->i_atime ) ; ++i ) {
			node->i_atime[i] = bd->readShort();
		}
		for (int i = 0; i < ARR_SIZE(node->i_atime); ++i ) {
			node->i_atime[i] = bd->readShort();
		}

		node->size = (node->i_size0 & 0xff) << 16 | (node->i_size1 & 0xffff );
		node->isDirectory = (((node->i_mode >> 14) & 1) == 1) && (((node->i_mode >> 13) & 1) != 1);
		node->isLarge = ((node->i_mode >> 12) & 1) == 1;
		list->push_back(std::move(node));
	}
	return std::move(list);
}

void Inode::createIndex( std::shared_ptr<BlockDevice> bd) { 
	table.reset (new StringIntMap());
	bool remaining = true;
	for (int i = 0; i < ARR_SIZE(i_addr); ++i ) {
		int offset = i_addr[i] * BlockDevice::BLOCK_SIZE;
		for (int j = 0; j < (BlockDevice::BLOCK_SIZE / Inode::DIRINFO_SIZE); ++j ) {
			if ( j >= ((size - ( i * BlockDevice::BLOCK_SIZE)) / Inode::DIRINFO_SIZE)) {
				remaining = false;
				break;
			}
			uint32_t p = j * Inode::DIRINFO_SIZE + offset ;

			bd->setIndex(p);
			int ino_num = bd->readShort();
			std::string name = bd->readString();
			if (ino_num != 0) {
				table->insert(std::make_pair(name, ino_num));
			}
		}
		if (!remaining) break;
	}
}

void Inode::showTable() {
	fprintf(stderr, "Block Off: " );
	for (int i = 0; i < ARR_SIZE(i_addr); ++i ){
		if (i * BlockDevice ::BLOCK_SIZE > size) break;
		fprintf(stderr, "0x%x, " , i_addr[i] * BlockDevice::BLOCK_SIZE);
	}
	fprintf(stderr, "\n");

	const StringIntMap:: const_iterator itEnd = table->end();
	for (StringIntMap:: const_iterator it = table->begin (); it != itEnd; ++it ) {
		fprintf(stderr, "%s : %d\n", it->first .c_str(), it->second);
	}
	fprintf(stderr, "\n");
}

std::string Inode::getChildren() {
	std::string s;
	for (auto it = table->begin (); it != table->end(); ++it){
		s += it->first ;
		s += "\n";
	}
	return s;
}


std::string Inode::getChildrenDetail (std::shared_ptr <std::vector <std::shared_ptr<Inode> > > nodes) {
	char buf [256];
	std :: string s;
	for (auto it = table->begin(); it != table->end(); ++it) {
		memset(buf, 0, 256);
		uint16_t i_mode = nodes->at(it->second)->i_mode;
		uint32_t size   = nodes->at(it->second)->size ;
		char dir = (nodes->at( it ->second)->isDirectory) ?'d':'-';
		char o_r = ((i_mode & 1) != 0) ? 'r' : '-';
		char o_w = ((i_mode & 2) != 0) ? 'w' : '-';
		char o_x = ((i_mode & 4) != 0) ? 'x' : '-';
		char g_r = ((i_mode & 0x8) != 0) ? 'r' : '-';
		char g_w = ((i_mode & 0x10) != 0) ? 'w' : '-';
		char g_x = ((i_mode & 0x20) != 0) ? 'x' : '-';
		char u_r = ((i_mode & 0x30) != 0) ? 'r' : '-';
		char u_w = ((i_mode & 0x40) != 0) ? 'w' : '-';
		char u_x = ((i_mode & 0x50) != 0) ? 'x' : '-';
		fprintf(stderr, "i_mode = %x\n", i_mode);
		sprintf(buf , "%c%c%c%c%c%c%c%c%c%c   %8d   %s\n" ,
				dir, u_r,u_w,u_x, g_r,g_w,g_x, o_r,o_w,o_x, size, it->first.c_str()); 
		s += buf;
	}
	return s;
}

int Inode::getNodeld(const std::string key) { 
	auto it = table->find(key);
	return (it != table->end()) ? (it->second) : -1;
}
