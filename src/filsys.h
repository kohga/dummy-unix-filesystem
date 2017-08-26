#ifndef __FILSYS_H__
#define __FILSYS_H__

#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

class BlockDevice;

#define ARR_SIZE(x) (int)(sizeof (x) / sizeof(x[0]))
#define FILE_NAME "./TestBlockDevice"

typedef std::map <std::string, int> StringIntMap;

class FilSys {
public :
	uint16_t s_isize;
	uint16_t s_fsize;
	uint16_t s_nfree;
	uint16_t s_free[100];
	uint16_t s_ninode;
	uint16_t s_inode[100];
	uint8_t s_flock;
	uint8_t s_ilock;
	uint8_t s_fmod;
	uint8_t s_ronly;
	uint16_t s_time[2];
	uint16_t pad[50];

	FilSys();
	~FilSys();
	static std::unique_ptr <FilSys> parse( std::shared_ptr <BlockDevice> bd);

	// for debug
	void show();
};

class BlockDevice {
private:
	uint8_t *data;
	std :: string path;
	uint32_t p;

public:
	const static uint32_t BLOCK_SIZE = 512;
	BlockDevice();
	BlockDevice(const std::string path);
	~BlockDevice();
	void setIndex(uint32_t p);

	// for debugging
	void show();

	uint16_t readShort();
	uint8_t readChar();
	std::string readString();
};

class Inode {
public:
	const static uint32_t INODE_SIZE = 32;
	const static uint32_t DIRINFO_SIZE = 16;
	uint16_t i_mode;
	uint8_t i_nlink;
	uint8_t i_uid;
	uint8_t i_gid ;
	uint8_t i_size0;
	uint16_t i_size1;
	uint16_t i_addr[8];
	uint16_t Latime [2];
	uint16_t i_atime[2];
	uint32_t size;
	bool    isDirectory;
	bool    isLarge;

	Inode();
	~Inode();
	static std::unique_ptr<std::vector<std::shared_ptr<Inode> > >
		parse(std::shared_ptr<BlockDevice> bd, uint16_t size);
	void createIndex(std::shared_ptr<BlockDevice> bd);

	void showTable();
	std::string getChildren();
	std::string getChildrenDetail(std::shared_ptr<std::vector<std::shared_ptr<Inode> > > nodes);
	int getNodeld(const std::string key);

private:
	std::unique_ptr<StringIntMap> table;
};

#endif /* FILSYS_H */
