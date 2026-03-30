# _This project has been created as part of the 42 curriculum by mabusham_

*GET_NEXT_LINE*
------------------------------------------------------------------------

##  Description

**get_next_line** is a C function that reads a file descriptor and returns one line at a time, including the newline character **\n** if present.

This implementation handles:
-   Multiple file descriptors simultaneously
-   Partial reads using a fixed **BUFFER_SIZE**
-   Persistent leftover data using static memory
-   Dynamic memory allocation without leaks

Each call returns the next line from the file until the end of file (EOF) is reached.

------------------------------------------------------------------------

##  Instructions

### Compilation

1. Clone or download the project:

``` bash
git clone <your-repo-url>
cd get_next_line
```

2. Compile with your main file

``` bash
cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c main.c -o gnl
```

3. Run

``` bash
./a.out
```

------------------------------------------------------------------------

##  Project Structure

| File | Responsibility |
|------|----------------|
| `get_next_line.c` | Main logic and reading flow |
| `get_next_line_utils.c` | Helper utility functions |
| `get_next_line.h` | Header file and prototypes |
| `get_next_line_bonus.c` | Main logic and reading flow (bonus version) |
| `get_next_line_utils_bonus.c` | Helper utility functions (bonus version) |
| `get_next_line_bonus.h` | Header file and prototypes (bonus version) |
------------------------------------------------------------------------

##  Core Functions

### get_next_line

**Prototype**

``` c
char *get_next_line(int fd);
```

**Parameters**
fd  - File descriptor to read from.

**Return Value**
Line - Next line from file.
NULL - End of file or error.

------------------------------------------------------------------------
##  Execution Flow



### append_to_line
**Prototype**
```c
char	*append_to_line(char *line, char *buf, int len);
```
**Role**\
*Append len characters from buf to line.*

*How it works?*
1. Allocate substr of size len
2. Copy part of buf into it
3. If ```line``` is empty, return substr
4. Else:
	Join ```line + substr``` and free old line and substr

Example:
```c
line = "Hel"
buf  = "lo"

result = "Hello"
```
Old memory is freed to prevent leaks.

------------------------------------------------------------------------
### process_stopper
**Prototype**
```c
int process_stopper(char **line, char *buf, char **remainder, int bytes);
```
**Role**\
*Check if \n exists in the buffer and handles it.*

If newline is found:
1. Add up to \n to line ```*line = append_to_line(*line, buf, index + 1);```
2. Store leftover in remainder ```*remainder = ft_strdup(buf + index + 1);```
3. Returns 1 to stop reading, 0 to continue and -1 for error.

Example:

```c
Buffer: "lo\nWo"

Line becomes: "Hello\n"
Remainder becomes: "Wo"
```
Stops reading.

------------------------------------------------------------------------
### handle_existing_remainder

**Prototype**

``` c
char	*handle_existing_remainder(char **remainder);
```

**Role**\
Use leftover data before reading from file\

If remainder contains a newline:
```c
remainder = "Hello\nWorld"
line = "Hello\n"
remainder = "World"
```

If no newline exists:
```c
remainder = "Hello"
line = "Hello"
remainder = NULL
```
Continue reading.

If \n exists in remainder
```c
remainder = "hello\nworld"
```
Extract
```c
line = "hello\n"
remainder = "world"
```
------------------------------------------------------------------------
### read_until

**Prototype**
```c
char	*read_until(int fd, char **remainder, char *buf);
```
**Role**\
Read chunks until you complete one line using:
```c
read(fd, buf, BUFFER_SIZE)
```
*How it works?*
1. Start with remainder ```line = handle_existing_remainder(remainder);```\
- If previous call had leftover text, use it first
```c
if (line && ft_strfind(line, '\n') >= 0)
	return (line);
```
- If that leftover already contains a newline, return immediately

2. Read loop
```c
while (1)
{
    bytes = read(fd, buf, BUFFER_SIZE);
```

**Error:**
```c
if (bytes < 0)
    return (free(line), NULL);
```

**End of file:**
```c
if (bytes == 0)
    break;
```
**Process buffer**
```c
res = process_stopper(&line, buf, remainder, bytes);
```

if (res == 1), newline found -> Stops while loop
if (res == -1), error -> return NULL

**No newline, append whole buffer**
```c
line = append_to_line(line, buf, bytes);
```

3. Final return 
```c
if (line && line[0] != '\0')
    return line;
else 
	return NULL;
```

------------------------------------------------------------------------
### get_next_line(fd)

**Prototype**

``` c
char *get_next_line(int fd);
```

**Role**
- Checks validity of file descriptor and buffer size
- Uses static remainder storage per file descriptor
``` c
static char *remainders[4096];
```
This allows handling multiple files simultaneously.

1. Allocate buffer
2. Call read_until ```line = read_until(fd, &remainders[fd]);```
3. If failure → clean remainder
4. Free buffer
5. Return the line

```
Hello\nWorld\n
```

------------------------------------------------------------------------

### Repeat until EOF

Example sequence:

File:
```c
Hello\nWorld\n42
```
Returns
```c
Call 1 → "Hello\n"
Call 2 → "World\n"
Call 3 → "42"
Call 4 → NULL
```
------------------------------------------------------------------------

##  Utilities

These helper functions support the main logic.

```c

//Returns length of string.
size_t ft_strlen(const char *str);

//Concatenates two strings.
char *ft_strjoin(char *s1, char *s2);

//Copies n characters.
char *ft_strncpy(char *dest, char *src, size_t n);

//Finds first occurence of character, Returns index or -1.
int	ft_strfind(char *str, char c);

//Duplicates string
char *ft_strdup(char *s1)

```
------------------------------------------------------------------------

##  Memory Management

### This implementation ensures:

- Proper allocation using **malloc**
- Proper deallocation using **free**
- No memory leaks
- Persistent remainder storage using static variable

------------------------------------------------------------------------

## Key Features

- Reads one line at a time
- Supports multiple file descriptors
- Uses static memory for persistence
- Efficient buffer handling
- Memory-safe implementation

------------------------------------------------------------------------

##  Example
### testfile1.txt
```txt
123456789
abcdef

This is file 1
aaaa


```
### testfile2.txt
```txt
This is file 2

test

hellooooooooooooo
```

### Test for one file

**main.c**
``` c
#include "get_next_line.h"

int main()
{
  int    fd;
  char  *next_line;
  int  count;

  count = 0;
  fd = open("testfile.txt", O_RDONLY);
  while (1)
  {
    next_line = get_next_line(fd);
    if (next_line == NULL)
      break ;
    count++;
    printf("[%d]:%s\n", count, next_line);
    next_line = NULL;
  }
  close(fd);
  return (0);
}
```
### Test for multiple files

**main.c**
``` c
#include "get_next_line.h"

int main()
{
   int		fd1;
	int		fd2;
	char	*line1;
	char	*line2;

	fd1 = open("testfile.txt", O_RDONLY);
	fd2 = open("testfile copy.txt", O_RDONLY);

	if (fd1 < 0 || fd2 < 0)
	{
		printf("Error opening files\n");
		return (1);
	}

	printf("=== Alternating Reads ===\n");

	line1 = get_next_line(fd1);
	printf("fd1: %s", line1);
	free(line1);

	line2 = get_next_line(fd2);
	printf("fd2: %s", line2);
	free(line2);

	line1 = get_next_line(fd1);
	printf("fd1: %s", line1);
	free(line1);

	line2 = get_next_line(fd2);
	printf("fd2: %s", line2);
	free(line2);

	close(fd1);
	close(fd2);
	return (0);
}
```

------------------------------------------------------------------------
## Summary
**get_next_line** provides a reliable and memory-safe way to read files line-by-line in C using buffered reads and persistent remainder storage.

------------------------------------------------------------------------
## Resources
- [YouTube] (https://youtu.be/8E9siq7apUU?si=aYWa_i9n3Xy9d0DJ)
- [Website] (https://medium.com/@beatrizbazaglia/get-next-line-3872eb3189e6)
- Use of AI: Explaining and Debugging