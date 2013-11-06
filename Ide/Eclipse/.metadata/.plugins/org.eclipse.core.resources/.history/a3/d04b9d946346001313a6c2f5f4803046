#import "Path.h"

bool Path::IsFileExists(const char *fileName)
{
	NSString *strFileName = [NSString stringWithUTF8String:(const char *)fileName];
	NSFileManager *fileMan = [[NSFileManager alloc] init];
	bool fileExists = [fileMan fileExistsAtPath:strFileName];
	[fileMan release];
//	[strFileName release];
	return fileExists;
}

void Path::GetAllFiles(std::vector<std::string> &files,
					   const std::string &path,
					   const std::string &filter)
{		
	NSFileManager *fileMan = [[NSFileManager alloc] init];
	NSError *err;
	NSArray *fileNames = [fileMan contentsOfDirectoryAtPath:(NSString *)[NSString stringWithUTF8String:(path.c_str())] error:(NSError **)&err];
	NSPredicate *filterPred = [NSPredicate predicateWithFormat:@"SELF like %@", [NSString stringWithUTF8String:(filter.c_str())]];
	NSArray *fileNamesFiltered = [fileNames filteredArrayUsingPredicate:filterPred];
	for (unsigned i = 0; i < [fileNamesFiltered count]; i++)
		files.push_back([(NSString *)[fileNamesFiltered objectAtIndex:i] UTF8String]);
	
	[fileMan dealloc];
}
