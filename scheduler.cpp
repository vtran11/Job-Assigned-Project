
#include "scheduler.h"
#include <iostream>
using namespace std;

Scheduler::Scheduler(int numJobs, int numWorkers, Job *jobs, int numPeople)
{
    
    int front=0; 	//start ptr
    int back=0; 	//finish ptr
    
    int *inDegree = new int[numJobs];
    int *slack = new int[numJobs];
    //int *critPath = new int[numJobs];
    
    int *topSort = new int[numJobs];
    
    for (int i=0; i<numJobs; i++)
    {
        inDegree[i]= 0;
    }
    
    
    
    
    // indegrees
    for (int i=0 ; i<numJobs ; i++)
    {
        if(jobs[i].numDependencies != 0)
        {
            
            for (int j=0 ; j<jobs[i].numDependencies; j++) //loop of dependencies
            {
                inDegree[jobs[i].dependencies[j]]++;
            }
        }
        
        else {
            inDegree[jobs[i].dependencies[0]]+= 0;
        }
    } //for
    
    
    // topological sort
    while (front!=numJobs) //while topsort[] isnt full
    {
        for (int i=0; i<numJobs; i++)
        {
            
            if (inDegree[i] == 0)
            {
                topSort[front] = i;
                inDegree[i] = -1;
                front++;
            }
            
        } //for
        
        for (int i=back; i< front; i++)
        {
            for (int j=0; j< (int)jobs[topSort[back]].numDependencies; j++)
            {
                inDegree[jobs[topSort[i]].dependencies[j]] --;
            }
            back++;
        }
    } //while
    
    
    ///////////////////critical path//////////////////////////////
    
    //earliest completition time
    jobs[topSort[0]].startTime = jobs[topSort[0]].length;
    
    for (int i= 0; i<numJobs ; i++)
    {
        for (int j=0; j<jobs[topSort[i]].numDependencies; j++)
        {
            int num = jobs[topSort[i]].dependencies[j];
            
            if (jobs[num].startTime == 0 )
            {
                jobs[jobs[topSort[i]].dependencies[j]].startTime = jobs[topSort[i]].startTime +
                jobs[jobs[topSort[i]].dependencies[j]].length;
            }
            
            else
            {
                int currentPath = jobs[topSort[i]].startTime + jobs[jobs[topSort[i]].dependencies[j]].length;
                
                if ( currentPath > jobs[jobs[topSort[i]].dependencies[j]].startTime)
                    currentPath = jobs[jobs[topSort[i]].dependencies[j]].startTime;
            }
        }
        
    }
    
    
    // latest completition time
    for( int i= numJobs; i > 0; i--)
    {
        jobs[topSort[0]].finishTime = jobs[topSort[0]].startTime;
        
        if(jobs[topSort[i]].numDependencies == 0)
            jobs[topSort[i]].finishTime = jobs[topSort[numJobs-1]].startTime;
        
        if(jobs[topSort[i]].numDependencies != 0)
        {
            int min = jobs[jobs[topSort[i]].dependencies[0]].finishTime -jobs[jobs[topSort[i]].dependencies[0]].length;
            
            
            for (int j=0; j<jobs[topSort[i]].numDependencies; j++)
            {
                if( min >= jobs[jobs[topSort[i]].dependencies[j]].finishTime -jobs[jobs[topSort[i]].dependencies[j]].length)
                {
                    jobs[topSort[i]].finishTime = jobs[jobs[topSort[i]].dependencies[j]].finishTime
                    - jobs[jobs[topSort[i]].dependencies[j]].length;
                }
                
                else
                    jobs[topSort[i]].finishTime = min;
            }
        }
        
        
        //slack
        //int critPtr=0;
        slack[i] = jobs[topSort[i]].finishTime - jobs[topSort[i]].startTime;
        
        for ( int i = 0 ; i < numJobs ; i++ )
        {
            jobs[topSort[i]].peopleIDs[0] = 0;  //first one
            jobs[topSort[i]].numPeopleUsed = 1;
        }
        
        //find people to replace zeros
        int person;
        
        for ( int i = 0 ; i < numJobs ; i++ )
        {
            person=1;
            for ( int j = i+1 ; j<numJobs ; j++ )
            {
                if ( (jobs[topSort[j]].peopleIDs[0] == 0 ) &&  (
                                                                ( ( jobs[topSort[j]].finishTime < jobs[topSort[i]].startTime ) && ( jobs[topSort[j]].finishTime < jobs[topSort[i]].finishTime ) ) ||
                                                                ( ( jobs[topSort[j]].startTime > jobs[topSort[i]].startTime ) && ( jobs[topSort[j]].startTime < jobs[topSort[i]].finishTime ) ) ||
                                                                ( ( jobs[topSort[j]].startTime < jobs[topSort[i]].startTime ) && ( jobs[topSort[i]].finishTime < jobs[topSort[j]].finishTime ) )  ||
                                                                ( ( jobs[topSort[i]].startTime < jobs[topSort[j]].startTime ) && ( jobs[topSort[j]].finishTime < jobs[topSort[i]].finishTime ) )
                                                                )
                    )
                    
                    
                {
                    
                    
                    
                    for ( int k = j-1 ; k>=0 ; k-- )
                    {
                        if ( jobs[topSort[k]].peopleIDs[0] == person  )
                        {
                            if( jobs[topSort[j]].finishTime <= jobs[topSort[k]].startTime )
                            {
                                jobs[topSort[i]].peopleIDs[0] = person;
                                jobs[topSort[i]].numPeopleUsed++;
                                
                                
                            }
                            else
                            {
                                person++;
                                k = j-1;
                                
                            }
                        }
                    }
                }
            }
        }

    } // Scheduler()
    
}
    
    void Scheduler::run()
    {
        
    } // run()
    
    
