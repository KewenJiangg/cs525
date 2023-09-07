#include"storage_mgr.h"
#include"dberror.h"

#include<string.h>
#include<stdio.h>


/* manipulating page files */
void initStorageManager(void) {}

RC createPageFile(char* fileName) {
	// initialization
	char initPage[PAGE_SIZE];
	// open new page
	FILE* newPage = fopen(fileName, "w+");
	if (newPage==NULL)
	{
		return RC_FILE_NOT_FOUND;
	}
	// fill page with '\0' 
	memset(initPage, '\0', PAGE_SIZE);
	// write page to file system
	int successNum = fwrite(initPage, sizeof(char), PAGE_SIZE, newPage);
	// close this file
	fclose(newPage);

	if (successNum<PAGE_SIZE)
	{
		return RC_WRITE_FAILED;
	}
	else
	{
		return RC_OK;
	}
}

RC openPageFile(char* fileName, SM_FileHandle* fHandle) {

	FILE* newPage = fopen(fileName, "r+");
	// ensure file is bot null
	if (newPage == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}
	// get file size and page number
	fseek(newPage, 0, SEEK_END);
	long fileSize = ftell(newPage);
	fHandle->totalNumPages = fileSize / PAGE_SIZE;
	fHandle->fileName = fileName;
	fHandle->curPagePos = 0;
	fHandle->mgmtInfo = newPage;

	return RC_OK;
}

RC closePageFile(SM_FileHandle* fHandle) {
	// checking 
	if (fHandle->mgmtInfo==NULL || fileno((FILE *)fHandle->mgmtInfo) == -1)
	{
		return RC_FILE_NOT_FOUND;
	}
	fclose((FILE *)fHandle->mgmtInfo);

	// prvent dangling pointer
	fHandle->mgmtInfo = NULL;
	return RC_OK;
}

RC destroyPageFile(char* fileName) {
	FILE* thisPage = fopen(fileName,"r+");
	if (thisPage == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}
	fclose(thisPage);
	remove(fileName);
	return RC_OK;
}

/* reading blocks from disc */
RC readBlock(int pageNum, SM_FileHandle* fHandle, SM_PageHandle memPage) {
	// ensure the input file is legal
	if (fHandle==NULL || fHandle->mgmtInfo == NULL)
	{
		return	RC_FILE_HANDLE_NOT_INIT;
	}
	if (pageNum>fHandle->totalNumPages-1 || pageNum<0)
	{
		return RC_READ_NON_EXISTING_PAGE;
	}
	// read block
	
	fseek((FILE *)fHandle->mgmtInfo, fHandle->curPagePos * PAGE_SIZE, SEEK_SET);
	fread(memPage, sizeof(char), PAGE_SIZE, (FILE *)fHandle->mgmtInfo);
	fHandle->curPagePos = pageNum;
	return RC_OK;
}

int getBlockPos(SM_FileHandle* fHandle) {
	// ensure the input file is legal
	if (fHandle == NULL || fHandle->mgmtInfo == NULL)
	{
		return	RC_FILE_HANDLE_NOT_INIT;
	}
	return fHandle->curPagePos;
}

RC readFirstBlock(SM_FileHandle* fHandle, SM_PageHandle memPage) {
	
	return readBlock(0, fHandle, memPage);
}

RC readPreviousBlock(SM_FileHandle* fHandle, SM_PageHandle memPage) {
	
	return readBlock(fHandle->curPagePos-1, fHandle, memPage);

}

RC readCurrentBlock(SM_FileHandle* fHandle, SM_PageHandle memPage) {

	return readBlock(fHandle->curPagePos, fHandle, memPage);

}

RC readNextBlock(SM_FileHandle* fHandle, SM_PageHandle memPage) {

	return readBlock(fHandle->curPagePos+1, fHandle, memPage);

}

RC readLastBlock(SM_FileHandle* fHandle, SM_PageHandle memPage) {

	return readBlock(fHandle->totalNumPages-1, fHandle, memPage);

}

/* writing blocks to a page file */
RC writeBlock(int pageNum, SM_FileHandle* fHandle, SM_PageHandle memPage) {
	// ensure all attributes is legal
	if (memPage==NULL || fHandle == NULL)
	{
		return RC_FILE_NOT_FOUND;
	} 
	if (fHandle->mgmtInfo == NULL)
	{
		return RC_FILE_HANDLE_NOT_INIT;
	}
	if (pageNum < 0 || pageNum > fHandle->totalNumPages - 1)
	{
		return RC_WRITE_FAILED;
	}
	
	
	// write block
	FILE* thPage = (FILE *)fHandle->mgmtInfo;
	fseek(thPage, pageNum * PAGE_SIZE, SEEK_SET);
	fwrite(memPage, sizeof(char), strlen(memPage), thPage);
	fHandle->curPagePos = pageNum;
	fclose(thPage);
	return RC_OK;
}


RC writeCurrentBlock(SM_FileHandle* fHandle, SM_PageHandle memPage) {

	return writeBlock(fHandle->curPagePos, fHandle, memPage);

}
RC appendEmptyBlock(SM_FileHandle* fHandle) {
	
	// ensure all attributes is legal
	if (fHandle == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}
	if (fHandle->mgmtInfo == NULL)
	{
		return RC_FILE_HANDLE_NOT_INIT;
	}

	fHandle->totalNumPages += 1;
	char temp[PAGE_SIZE];
	memset(temp, '\0', PAGE_SIZE);
	return writeBlock(fHandle->totalNumPages - 1, fHandle, temp);
}

RC ensureCapacity(int numberOfPages, SM_FileHandle* fHandle) {
	// ensure all attributes is legal
	if (fHandle == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}
	if (fHandle->mgmtInfo == NULL)
	{
		return RC_FILE_HANDLE_NOT_INIT;
	}
	// confirm the number of pages to be added
	int addPage = numberOfPages + 1 - fHandle->totalNumPages;
	if (addPage>0)
	{
		for (int i = 0; i < addPage; i++)
		{
			int flag = appendEmptyBlock(fHandle);
			if (flag)
			{
				return RC_WRITE_FAILED;
			}
		}
	}
	return RC_OK;
}


