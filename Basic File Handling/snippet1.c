#include <stdio.h>
#include <unistd.h> 
#include <errno.h>
int main (int argc, char* argv[])
{ 
	char* filepath = argv[1]; 
// Assign returnval with access(filepath,F_OK) to check if the file exists
// 0 means exist, -1 means not and set the corresponding errno
	int returnval = access(filepath,F_OK);
if(returnval == 0) 
	printf ("\n %s exists\n", filepath); 
else { if (errno == ENOENT) printf ("%s does not exist\n", filepath);
else if (errno == EACCES) printf ("%s is not accessible\n",filepath);
return 0;
}

// Check read access using similar method...
if(access(filepath, R_OK) == 0 )
	printf("\n %s is readable \n",filepath);
else{// if access(filepath,R_OK)!=0, means the file is not readable
	printf("\n %s is not readable \n",filepath);

}

// Check write access ...
if(access(filepath,W_OK) == 0 )
	printf("\n %s is writeable \n",filepath);
else{
	printf("\n %s is not writeable \n",filepath);
}

return 0;
}