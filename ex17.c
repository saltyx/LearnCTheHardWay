
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define MAX_DATA 512
#define MAX_ROWS 100
struct Address
{
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};
struct Database{
	struct Address rows[MAX_ROWS];	
};
struct Connection{
	FILE *file;
	struct Database *db;
};



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  die
 *  Description:  
 * =====================================================================================
 */
	void 
die ( const char*message )
{
	if(errno)
	{
		perror(message);
	}
	else{
		printf("ERROR: %s\n", message);
	}
	exit(1);
}		/* -----  end of function die  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Address_print
 *  Description:  
 * =====================================================================================
 */
	void
Address_print ( struct Address *addr )
{
	printf("%d %s %s\n", addr->id, addr->name, addr->email);

}		/* -----  end of function Address_print  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Database_load()
 *  Description:  
 * =====================================================================================
 */
	void
Database_load ( struct Connection *conn )
{
	int rc = fread(conn->db, sizeof(struct Database), 1,conn->file );
	if (rc!=1)
		die("Fail to load databse\n");
}		/* -----  end of function Database_load()  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Database_open
 *  Description:  
 * =====================================================================================
 */
struct Connection*
Database_open ( const char* filename, char mode )
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn)die("memory ERROR");

	conn->db = malloc(sizeof(struct Database));
	if (!conn->db) die("memory ERROR");

	if(mode == 'c')
	{
		conn->file = fopen(filename, "w");
	}
	else {
		conn->file = fopen(filename, "r+");

		if(conn->file)
		{
			Database_load(conn);
		}
	}
	if(!conn->file) die("Fail to Open File");

	return conn;
}		/* -----  end of function Database_open  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Database_close
 *  Description:  
 * =====================================================================================
 */
	void
Database_close ( struct Connection*conn )
{
	if(conn){
		if(conn->file)fclose(conn->file);
		if(conn->db)free(conn->db);
		free(conn);
	}
}		/* -----  end of function Database_close  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Database_write
 *  Description:  
 * =====================================================================================
 */
	void
Database_write ( struct Connection *conn )
{
	rewind(conn->file);
	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc!=1)
		die("Fail to write databse.");
	rc = fflush(conn->file);
	if(rc == -1)
		die("Cannot flush database");
}		/* -----  end of function Database_write  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Database_create
 *  Description:  
 * =====================================================================================
 */
	void
Database_create ( struct Connection*conn )
{
	int i=0;
	for(i=0;i<MAX_ROWS;i++){
		struct Address addr = { .id=i, .set=0 };
		conn->db->rows[i] = addr;
	}
		
}		/* -----  end of function Database_create  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Database_set
 *  Description:  
 * =====================================================================================
 */
	void
Database_set ( struct Connection*conn, int id, const char*name, const char *email )
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set)die("Already set ,delete it first");

	addr->set = 1;
	char *res = strncpy(addr->name, name, MAX_DATA);
	if(strlen(addr->name) >= MAX_DATA)
		printf("WARNING:length_error\n");
	res[MAX_DATA-1] = '\0';

	if(!res) die("Name copy failed");
	printf("name is %s\n", addr->name);
	char* eres = strncpy(addr->email, email, MAX_DATA);
	if(!eres) die("Email copy failed");	
	if(strlen(addr->email) >= MAX_DATA)
		printf("WARNING:length_error\n");
	
	eres[MAX_DATA-1] ='\0';
	printf("email is %s\n", addr->email);
}		/* -----  end of function Database_set  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Database_get
 *  Description:  
 * =====================================================================================
 */
	void
Database_get ( struct Connection *conn, int id )
{
	struct Address *addr = &conn->db->rows[id];

	if(addr->set){
		Address_print(addr);
	}else{
		die("ID IS NOT SET");
	}
}		/* -----  end of function Database_get  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Database_delete
 *  Description:  
 * =====================================================================================
 */
	void
Database_delete ( struct Connection *conn, int id )
{
	struct Address addr = {.id = id, .set=0};
	conn->db->rows[id] = addr;
}		/* -----  end of function Database_delete  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Database_list
 *  Description:  
 * =====================================================================================
 */
	void
Database_list (struct Connection *conn)
{
	int i=0;
	struct Database *db = conn->db;

	for (i=0;i<MAX_ROWS;i++)
	{
		struct Address *cur = &db->rows[i];

		if(cur->set){
			Address_print(cur);
		}
	}
}		/* -----  end of function Database_list  ----- */


#include	<stdlib.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	if(argc<3) die("USAGE:ex17 <dbfile> <action>[action params]");
	char *filename = argv[1];
	char action = argv[2][0];
//	printf("mode is %c" ,action);//
//	printf("filename is %s", filename);//
	struct Connection *conn = Database_open(filename,action);
	int id=0;

	if(argc > 3)id=atoi(argv[3]);
	if(id >= MAX_ROWS) die("this is not that many record");

	switch(action){
		case 'c':
			Database_create(conn);
			Database_write(conn);
			break;
		case 'g':
			if(argc!=4)die("need an id to get");
			Database_get(conn, id);
			break;
		case 's':
			if(argc != 6)die("need id name email to set");
			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;
		case 'd':
			if(argc !=4)die("need id to delete");
			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		default:
			die("invalid action, only c:create, g=get,s=set,d=del, l=list");
	}
	Database_close(conn);
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */


