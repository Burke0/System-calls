/*
 * SystemCalls.cpp
 *
 *
 *      Author: J. Alex Burke
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <pwd.h>




int main()//int argc, char **argv)

{
	int 			fd, fdcopy, id;
	const char 		msg[] = "Success";
	std::string 	pathname, pathnameCopy, homedir;
	const char*		pathnameFirefox= "/usr/bin/firefox";
	char*			parmList[] = {"firefox", "www.fairmontstate.edu", NULL};
	char 			buff[20];
	struct passwd *	pw;
	struct stat 	stat;
	loff_t 			len, ret;


	pw= getpwuid(getuid());
	homedir = pw->pw_dir; //get home directory
	pathname =homedir + "/aaatestfile.txt";
	pathnameCopy= "/home/alex/aaatestfile(1).txt";

	fd	= open(pathname.c_str() ,O_CREAT|O_RDWR, S_IRWXU);//, S_IRWXU );
	//S_IRWXU  00700 user (file owner) has read,  write,  and  execute permission
	//create/ read and write flags
	write(fd, msg, sizeof(msg)-1); //write success to file

	close(fd);//close and reopen file so cache is cleared. Read only
	fd=open(pathname.c_str(), O_RDONLY, S_IRWXU);

	len = stat.st_size;
	fdcopy= open(pathnameCopy.c_str(), O_CREAT | O_WRONLY, S_IRWXU); //open second file, write only
	do //copy file.
	{
		ret = copy_file_range(fd, NULL, fdcopy, NULL, len, 0);
		std::cout<<"bytes copied "<<ret<<std::endl;
	    if (ret == -1)
	    {
	      perror("copy_file_range");
	      exit(EXIT_FAILURE);
	    }
	    len -= ret;
	 } while (len > 0 && ret > 0);

	close(fd);//close and reopen file so cache is cleared. Read only
	fd=open(pathname.c_str(), O_RDONLY, S_IRWXU);
	std::cout<<read(fd,buff,10)<<" bytes read: " <<buff<<std::endl; //read into buffer and output to console
	close(fdcopy);
	remove(pathnameCopy.c_str());
	close(fd);//close files


	id = fork();
	if (id > 0)
	{
		//parent process
		std::cout<<"Firefox Started! pid: "<< getpid()<<std::endl;
	}
	else if (id == 0) {
		//child process
		std::cout<<execv(pathnameFirefox, parmList)<<std::endl;
	}

	return 0;
}
