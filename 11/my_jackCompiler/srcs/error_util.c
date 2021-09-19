#include "jackCompiler.h"

int					print_error(t_err err)
{
	if (err == ERR_NO_ARG)
		printf("Error: Please provide a .jack file or directory\n");
	if (err == ERR_OPEN_FAIL)
		perror("Error");
		// printf("Error: Can't open the file\n");
	if (err == ERR_OPEN_DIR_FAIL)
		perror("Error");
		// printf("Error: Can't open the directory\n");
	if (err == ERR_WRONG_EXT)
		printf("Error: Input file must have .jack extension\n");
	if (err == ERR_DST_FILE)
		printf("Error: Can't create output file .vm\n");
	else
		printf("Error: Unknown error occurred\n");
	return (0);
}
