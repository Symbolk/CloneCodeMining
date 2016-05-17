// PrefixSpan algorithm
// August 2000

#include <stdlib.h>

#include "Global.h"
#include "ProjDB.h"

// Added by Ramin
#if defined( _FIND_MAX_SEQS )
SeqTree * MainSeqTree;
#endif // defined( _FIND_MAX_SEQS )

#if defined( _FIND_CLOSED_SEQS )
SequenceList * aSeqList;
#endif
/////////////////
// Added by Xifeng
#if defined (_ANOTHER_CLOSED_APPROACH)
TreeNode *root;
TreeNode *currentLevel;
ReverseHashTable *reverseTable; // from NumOfItems to TreeNode*, multimap
int closed_maxFreq[256];
#endif
////////////////


void InitApp(const char* filename, const double dSupport, const int nItemCount){

	CreateTimers(3);
  gN_ITEMS = (nItemCount+1); // because it is zero based.
  gpErrFile = file_open(ERRFILE, "w");
  int nFileSize = InitProjDB(filename);

#if defined( _WRITE_FREQUENT_FILE )
  gpFreqFile = file_open(FREQUENT, "w"); 
#endif
  //gpResultFile = file_open(RESULTS, "w");
  //gpStatusFile = file_open(STATUS, "w");
  gpResultFile = gpStatusFile = stdout;

	//Added by Xifeng
#if defined (_ANOTHER_CLOSED_APPROACH)
	root = new TreeNode(-1, 0);
	root -> ItemsetNumber=0;
	root->Parent = NULL;
	currentLevel = root;

	reverseTable = new ReverseHashTable;
	for(int i=0; i< 256; i++)
		closed_maxFreq[i]=0;
#endif


	// Added by Ramin
#if defined( _FIND_MAX_SEQS )
	fprintf( gpStatusFile, "Modified 1LPrefixSpan for finding Max sequential patterns" );
	#if defined( _DO_NAIVE_APPROACH )
		fprintf( gpStatusFile, " (Naive approach).\n" );
	#else
		fprintf( gpStatusFile, " (MaxSequence).\n" );
	#endif
#endif // defined( _FIND_MAX_SEQS )

#if defined( _FIND_CLOSED_SEQS )
	fprintf(gpStatusFile, "Modified 1LPrefixSpan for finding Closed sequential patterns." );
	#if defined( _DO_NAIVE_APPROACH )
		fprintf( gpStatusFile, " (Naive approach).\n" );
	#else
		fprintf( gpStatusFile, " (ClosedSequence).\n" );
	#endif
/*
	#if _FIND_CLOSED_SEQS == _NAIVE_APP
		fprintf(gpStatusFile, "( Naive approach ).\n" );
	#else
		fprintf(gpStatusFile, "( Find Common Prefix approach )" );
		#if defined( _USE_OUTPUT_BUFFER )
			fprintf( gpStatusFile, " ( Using output buffer )" );
		#endif
		#if defined( _NEW_SEQUENCE_LIST )
			#if _NEW_SEQUENCE_LIST==10
				fprintf( gpStatusFile, " ( Sorted seq list )" );
			#else if _NEW_SEQUENCE_LIST==20
				fprintf( gpStatusFile, " ( Sequence tree )" );
			#endif
		#else
			fprintf( gpStatusFile, " ( Naive seq list )" );
		#endif
		fprintf( gpStatusFile, ".\n" );
	#endif
*/
#endif //  defined( _FIND_CLOSED_SEQS )
	/////////////////

#ifdef DISK_BASED
	fprintf(gpStatusFile, "clospan: A sequential pattern mining algorithm (pseudo projection).\n");
#else
	fprintf(gpStatusFile, "clospan: A sequential pattern mining algorithm.\n");
#endif
	fprintf(gpStatusFile, "Implemented by Behzad Mortazavi-Asl, in IDBL, SFU\n");
	fprintf(gpStatusFile, "All rights reserved!\n");
	fprintf(gpStatusFile, "Data set (%.3f MB): %s\n", 
          nFileSize/(1024.0*1024.0), filename);
	fprintf(gpStatusFile, "# of items: %d\n", nItemCount);
	fprintf(gpStatusFile, "Support threshold: %.3f%%\n", dSupport*100);
  fprintf(gpStatusFile, "-----------------------\n");
}

void CloseApp(){

	DeleteTimers();
#if defined( _WRITE_FREQUENT_FILE )
  fclose (gpFreqFile);
#endif
  //fclose (gpResultFile);
  //fclose (gpStatusFile);
  freemem ((void**) &gnArrLargeCount);
#if defined( _FIND_CLOSED_SEQS )
	freemem ((void**) &gnResSizeCount);
#endif
	freemem ((void**) &buf_idx);
#ifndef DISK_BASED
	freemem ((void**) &bufseq);
#endif
  CloseProjDB();
  fclose (gpErrFile);
}

void PrefixSpan(struct PROJ_DB *pDB){

	int i=0, j=0, nFreqCount=0;

	//add by Xifeng, currentLevel will be modified to next level
	#if defined (_ANOTHER_CLOSED_APPROACH)
		if (addSequence(pDB, &currentLevel, reverseTable)==EQUAL_PROJECT_DB_SIZE) {
			clean_projcted_db(pDB, &nFreqCount);
			return;	
		}
	#endif

	// scan sequence database once, find length-1 sequences
  struct PROJ_DB *proj_db=make_projdb_from_projected_db(pDB, &nFreqCount);


	if (nFreqCount>0)
	{
    for (i=0; i<nFreqCount; i++)
		{

      if( proj_db[i].m_nSup < gSUP )
			{

				//Added by Ramin
#if defined( _USE_OUTPUT_BUFFER )
				//fprintf( gpFreqFile, "+===>>>>>>  Right Pattern  " );
				//(*proj_db[i].OutputBuf).Print( gpFreqFile );
				EmptyBuffer( aSeqList, proj_db[i].OutputBuf );
#endif
#if defined( _FIND_MAX_SEQS ) && !defined( _DO_NAIVE_APPROACH )
				//Sequence * aSeq = new Sequence( proj_db[i].m_pnPat, proj_db[i].m_nPatLen, proj_db[i].m_nSup );
				Sequence * aSeq = new Sequence( &(proj_db[i]), proj_db[i].m_nMaxSup );
				(*MainSeqTree).AddSeq( aSeq );
#endif
				//added by xifeng

				n_total_mem-=(proj_db[i].m_nPatLen*sizeof(int));
        freemem ((void**) &(proj_db[i].m_pnPat));
        for (j=0; j<proj_db[i].m_nSup; j++) 
				{
#ifndef DISK_BASED
					n_total_mem-=proj_db[i].m_nSeqSize;
          freemem ((void**) &(proj_db[i].m_pProjSeq[j].m_ppSeq[0]));
#endif
					if (proj_db[i].m_pProjSeq[j].m_nProjCount > 1) 
					{
						n_total_mem-=(proj_db[i].m_pProjSeq[j].m_nProjCount*sizeof(int*));
						freemem ((void**) &(proj_db[i].m_pProjSeq[j].m_ppSeq));
					}
				}
				n_total_mem-=(proj_db[i].m_nMaxSup*sizeof(struct PROJ_SEQ));
        freemem ((void**) &(proj_db[i].m_pProjSeq));

      } else n_proj_db++;
    }
    for (i=0; i<nFreqCount; i++)
		{
      if (proj_db[i].m_nSup >= gSUP) 
			{
        PrefixSpan (&(proj_db[i]));
			}
#if defined (_ANOTHER_CLOSED_APPROACH)
			else {
				if (addSequence(&proj_db[i], &currentLevel, reverseTable) != EQUAL_PROJECT_DB_SIZE) 
					currentLevel = currentLevel->Parent;
			}
#endif
    }
		n_total_mem-=(nFreqCount*sizeof(struct PROJ_DB));
    freemem ((void**) &proj_db);
	}
	//add by Xifeng, currentLevel will be modified to its parent level
	#if defined (_ANOTHER_CLOSED_APPROACH)
		currentLevel = currentLevel->Parent;
	#endif
}

int main(int argc, char** argv)
{ 


  if (argc != 4)
	{
    gpErrFile = file_open(ERRFILE, "w");
    if (gpErrFile!=NULL)
		{
      fprintf (gpErrFile, "Usage: clospan <filename> <support> <itemcount>\n");
      fclose (gpErrFile);
    }
    printf ("Usage: clospan  <filename> <support> <itemcount>\n");
    exit (-1);
  }
  InitApp(argv[1], atof(argv[2]), atoi(argv[3]));
	ResetTimer(0);

	// Added by Ramin
#if defined( _FIND_MAX_SEQS )
	MainSeqTree = new SeqTree( atoi(argv[3]) );
#endif // defined( _FIND_MAX_SEQS )

#if defined( _FIND_CLOSED_SEQS )
	#if _NEW_SEQUENCE_LIST == 20
		aSeqList = new SequenceList( gN_ITEMS );
	#else
		aSeqList = new SequenceList();
	#endif
#endif
	/////////////////

	// added by Ramin for test
	//Test();
	//RTATest( NULL );


	int i=0, j=0, nFreqCount=0;
	// scan sequence database once, find length-1 sequences
  struct PROJ_DB *proj_db=make_projdb_from_org_dataset(atof(argv[2]), &nFreqCount);


	if (nFreqCount>0)
	{
    for (i=0; i<nFreqCount; i++)
		{
      if (proj_db[i].m_nSup < gSUP)
			{
//added by xifeng
#if defined (_ANOTHER_CLOSED_APPROACH)
				if (addSequence(&proj_db[i], &currentLevel, reverseTable) != EQUAL_PROJECT_DB_SIZE) 
					currentLevel = currentLevel->Parent;				
#endif
        for (j=0; j<proj_db[i].m_nSup; j++) 
				{

#ifndef DISK_BASED
					n_total_mem-=proj_db[i].m_nSeqSize;
          freemem ((void**) &(proj_db[i].m_pProjSeq[j].m_ppSeq[0]));
#endif
					if (proj_db[i].m_pProjSeq[j].m_nProjCount > 1) {
						n_total_mem-=(proj_db[i].m_pProjSeq[j].m_nProjCount*sizeof(int*));
						freemem ((void**) &(proj_db[i].m_pProjSeq[j].m_ppSeq));
					}
				}
				n_total_mem-=(proj_db[i].m_nMaxSup*sizeof(struct PROJ_SEQ));
        freemem ((void**) &(proj_db[i].m_pProjSeq));
      } else n_proj_db++;
    }
    for (i=0; i<nFreqCount; i++)
		{
      if (proj_db[i].m_nSup >= gSUP)
			{ 
        PrefixSpan (&(proj_db[i]));
        //fprintf(gpStatusFile, "|");
      } 
#if defined( _USE_OUTPUT_BUFFER )
			else { // Else part added by Ramin
				//fprintf( gpFreqFile, "+===>>>>>>  Right Pattern  " );
				//(*proj_db[i].OutputBuf).Print( gpFreqFile );

				EmptyBuffer( aSeqList, proj_db[i].OutputBuf );
			}
#endif
#if defined( _FIND_MAX_SEQS ) && !defined( _DO_NAIVE_APPROACH )
			else { // Else part added by Ramin
				//Sequence * aSeq = new Sequence( proj_db[i].m_pnPat, proj_db[i].m_nPatLen, proj_db[i].m_nSup );
				Sequence * aSeq = new Sequence( &(proj_db[i]), proj_db[i].m_nMaxSup );
				(*MainSeqTree).AddSeq( aSeq );
			}
#endif

    }
		n_total_mem-=(nFreqCount*sizeof(struct PROJ_DB));
    freemem ((void**) &proj_db);
	}

	double TimeDiff = GetTimeDiff(0);
  fprintf(gpResultFile, "%.3f seconds (Total running time)\n", TimeDiff );
  for (i=1; gnArrLargeCount[i]>0; i++)
	{
		fprintf(gpResultFile, "Large %d : %d\n", i, gnArrLargeCount[i]);
		gnArrLargeCount[0]+=gnArrLargeCount[i];
	}

  fprintf(gpResultFile, "Total of %d large items.\n", gnArrLargeCount[0]);
	fprintf(gpResultFile, "# of projected datasets: %d\n", n_proj_db);
	fprintf(gpResultFile, "Maximum memory usage: %.3fMB\n", 
		double(n_max_mem)/(1024.0*1024.0));

	// Added by Ramin
#if defined( _FIND_MAX_SEQS )
	FILE *myFile = NULL;
  myFile = file_open( "Maxset.txt", "w" ); 
	(*MainSeqTree).PrintRules( myFile );
  fprintf( myFile, "%.3f seconds (Total running time)\n", TimeDiff );
  fprintf( myFile, "Total of %d large items.\n", gnArrLargeCount[0] );
	fprintf( myFile, "# of projected datasets: %d\n", n_proj_db );
	fprintf( myFile, "Maximum memory usage: %.6fMB\n", double(n_max_mem)/(1024.0*1024.0) );
	delete MainSeqTree;
  fprintf( myFile, "%.3f seconds (Total running time after sort, and output.)\n", GetTimeDiff(0) );
  fclose( myFile );
#endif // defined( _FIND_MAX_SEQS )

#if defined( _FIND_CLOSED_SEQS )
	FILE *myFile = NULL;
  myFile = file_open( "Closedset.txt", "w" ); 
	(*aSeqList).Print( myFile );

  fprintf( myFile, "%.3f seconds (\nTotal running time)\n\n", TimeDiff );

  for ( i=1; i < gMAX_PAT_LEN; i++ )
	{
		if( gnResSizeCount[i] > 0 )
			fprintf( myFile, "Closed %d : %d\n", i, gnResSizeCount[i] );
	}

  fprintf( myFile, "\nTotal of %d large items.\n", gnArrLargeCount[0] );
	fprintf( myFile, "# of projected datasets: %d\n", n_proj_db );
	fprintf( myFile, "Maximum memory usage: %.6fMB\n", double(n_max_mem)/(1024.0*1024.0) );
	delete aSeqList;
  fprintf( myFile, "%.3f seconds (Total running time after sort, and output.)\n", GetTimeDiff(0) );
  fclose( myFile );
#endif

  //added by Xifeng
#if defined (_ANOTHER_CLOSED_APPROACH)
	NodeVector::iterator it, endit;
	for (it=root->Children->begin(), endit=root->Children->end(); it != endit; it++) {
		//if ((*it)->Item==6010)
			//printf("GAGAGA\n");
		closed_maxPruning((*it), root);
	}
	FILE *closed_maxFile = NULL;
	closed_maxFile = file_open( "ClosedMaxset.txt", "w" );

#if defined (_ANOTHER_MAX_APPROACH)
	fprintf(closed_maxFile, "MAX\n");
#else
	fprintf(closed_maxFile, "CLOSED\n");
#endif

	for (it=root->Children->begin(), endit=root->Children->end(); it != endit; it++) {
		if ((*it)->Parent == root)
			(*it)->Print("(", closed_maxFile);
	}

	fprintf( closed_maxFile, "%.3f seconds (\nTotal running time)\n\n", TimeDiff );

	fprintf(closed_maxFile, "Total # of TreeNode: %d\n", zzz);

 
	for (i=0; i<256; i++)
		if (closed_maxFreq[i] != 0)
			fprintf(closed_maxFile, "Closed/Max %d : %d\n", i, closed_maxFreq[i]);
	
	fprintf(closed_maxFile, "\nTotal of %d large items.\n", gnArrLargeCount[0] );
	fprintf(closed_maxFile, "# of projected datasets: %d\n", n_proj_db );
	fprintf(closed_maxFile, "Maximum memory usage: %.6fMB\n", double(n_max_mem)/(1024.0*1024.0) );
	
  fprintf(closed_maxFile, "%.3f seconds (Total running time after sort, and output.)\n", GetTimeDiff(0) );
  fclose(closed_maxFile );
#endif

	/////////////////
  fprintf( gpResultFile, "%.3f seconds (Total running time after sort, and output.)\n", GetTimeDiff(0) );

  CloseApp();
  return 0;
}

//////////////////////////////////////////////////////////////////////
// END
