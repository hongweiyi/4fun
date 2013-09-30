package com.hongweiyi;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.Text;

/**
 * 
 * Every file as a value, filename as a key. Merge small to a sequencefile.
 * 
 * @author Wikie
 */
public class MergeFileNio {
	private static Text key = new Text();
	private static Text value = new Text();
	private static List<String> fileList = new ArrayList<String>();

	private static int curProgress = 0;
	private static int PROGRESS_STEP = 5;

	public static void main(String[] args) throws IOException {
		if (args.length != 3) {
			System.out.println("Usage: jar data_dir hdfs_path dir_depth");
			System.exit(-1);
		}

		try {
			tree(new File(args[0]), 0, Integer.parseInt(args[2]));
		} catch (NumberFormatException e) {
			System.out.println("argv[3] must be a integer");
			System.exit(-1);
		} catch (Exception e) {
			e.printStackTrace();
			System.exit(-1);
		}

		long start = System.currentTimeMillis();
		merge(args[1]);
		long end = System.currentTimeMillis();
		System.out.println(end - start);
	}

	/**
	 * Merge files to hdfs
	 * 
	 * @param seqPath
	 *            : hdfs location
	 * @throws IOException
	 */
	public static void merge(String seqPath) throws IOException {
		Configuration conf = new Configuration();
		FileSystem fs = FileSystem.get(conf);

		Path seqFile = new Path(seqPath);
		SequenceFile.Writer writer = new SequenceFile.Writer(fs, conf, seqFile,
				Text.class, Text.class);

		// load buffer
		int SIZE = 64 * 1024;
		// byte[] barray = new byte[SIZE];

		// file buffer
		FileBytesBuffer fileBytes = new FileBytesBuffer();
		int cur = 0;

		System.out.println("\n" + new Date() + " INFO merge: Start! ");
		System.out.println("===============================================");
		progress(cur);
		for (String path : fileList) {
			String fileName = path.substring(path.lastIndexOf(File.separator));
			key.set(fileName);

			FileInputStream f = new FileInputStream(path);
			FileChannel ch = f.getChannel();
			MappedByteBuffer mb = ch.map(FileChannel.MapMode.READ_ONLY, 0L,
					ch.size());

			int nRead;
			int current = 0;
			while (mb.hasRemaining()) {
				nRead = Math.min(mb.remaining(), SIZE);
				mb.get(fileBytes.getBytes(current + nRead), current, nRead);
				// System.arraycopy(barray, 0,
				// fileBytes.getBytes(current + nRead), current, nRead);
				current += nRead;
			}
			value.set(fileBytes.getBytes(), 0, current);
			writer.append(key, value);

			ch.close();
			f.close();

			progress(++cur);
		}

		IOUtils.closeStream(writer);

		System.out.println("===============================================");
		System.out.println(new Date() + " INFO merge: Done! \n");
	}

	/**
	 * 
	 * Report progress
	 * 
	 * @param cur
	 *            : current progress
	 */
	private static void progress(int cur) {
		int curPercent = (int) (100 * ((double) cur / (double) fileList.size()));
		if (curPercent - curProgress >= PROGRESS_STEP) {
			System.out.print(new Date() + " INFO merge progress: " + curPercent
					+ "%\r");
			curProgress = curPercent;
		}
		if (cur == 0)
			System.out.print(new Date() + " INFO merge progress: " + 0 + "%\r");
		if (curProgress == 100)
			System.out.println();
	}

	/**
	 * get all file in directory
	 * 
	 * @param dir
	 *            : input directory
	 * @param curDepth
	 *            : current depth, start with zero
	 * @param depthLimit
	 *            : depth limit
	 */
	private static void tree(File dir, int curDepth, int depthLimit) {
		File[] childs = dir.listFiles();
		if (null == childs)
			return;
		for (File file : childs) {
			if (curDepth == depthLimit) {
				fileList.add(file.getAbsolutePath());
			}
			tree(file, curDepth + 1, depthLimit);
		}
	}
}
