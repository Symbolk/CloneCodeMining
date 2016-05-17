import java.io.*;
import java.util.*;

public class test{
	private static ArrayList<String> getFilePathList(File dir){
		ArrayList<String> pathList = new ArrayList<String>();
		File[] files=dir.listFiles();
		     for (int i = 0;i < files.length; i++) {
      		File temp=files[i];
      		if(temp.isDirectory()){
      			pathList.addAll(getFilePathList(temp));
      		}else{
      			if(temp.getAbsolutePath().endsWith(".java")){
      				pathList.add(temp.getAbsolutePath());
      			}

      		}
      }
		return pathList;
	}

	public static void main(String[] args) throws Exception{
	/*	System.out.println(System.getProperty("user.dir"));
		System.out.println(Thread.currentThread().getContextClassLoader().getResource(""));
		System.out.println(test.class.getClassLoader().getResource(""));    
		System.out.println(ClassLoader.getSystemResource(""));
		System.out.println(test.class.getResource(""));   
		System.out.println(test.class.getResource("/")); 
		System.out.println(new File("/").getAbsolutePath()); */
		String projectName=new String("PatchFilter");
		// File outfolder=new File("D:\\"+projectName);
		// outfolder.mkdir();
		ArrayList<String> pathList = new ArrayList<String>();
		File project=new File("C:\\Users\\SymbolK\\Desktop\\CloneCode\\PatchFilter\\src");
		pathList = getFilePathList(project);     
   		    
      for(String filepath : pathList){
      	StringTokenizer stz=new StringTokenizer(filepath,"\\");
      	boolean seeProject=false;
      	boolean isFile=false;
      	//get the relative path and build the outfile name
      	String outfile="";
      	while(stz.hasMoreTokens()){
      		String temp=stz.nextToken();
      		if(temp.equals(projectName)){
      			seeProject=true;
      			continue;
      		}
      		if(temp.endsWith(".java")){
      			isFile=true;
      		}
      		if(seeProject){
      			if(!isFile){
      				outfile=outfile+temp+"@";
      			}else{
      				outfile=outfile+temp;
      			}
      		}
      	}
      	System.out.println(outfile);
      }
	}
}