package com.hongweiyi;

/**
 * 
 * FileBytesBuffer
 *  
 * For maintaining space in memory
 * The maximum to avoid GC.
 * 
 * @author Wikie
 * 
 */
public class FileBytesBuffer {
	private byte[] bytes;
	private int length;

	private static int PRE_ALLOC_BYTES = 10 * 1024 * 1024;

	public FileBytesBuffer(int bytes_num) {
		bytes = new byte[bytes_num];
		length = PRE_ALLOC_BYTES;
		PRE_ALLOC_BYTES = bytes_num;
	}

	/**
	 * pre alloc 10MB space for byte array
	 * 
	 */
	public FileBytesBuffer() {
		this(PRE_ALLOC_BYTES);
	}

	/**
	 * get this obj's byte array
	 * 
	 * @param requestLen
	 *            : if requestLen greater than this.length then alloc a new
	 *            space for this.bytes
	 * 
	 * @return old bytes or a new alloced bytes
	 */
	public byte[] getBytes(int requestLen) {
		if (requestLen > this.length) {
			byte[] newBytes = new byte[requestLen];
			System.arraycopy(bytes, 0, newBytes, 0, bytes.length);

			this.bytes = newBytes;
			this.length = requestLen;
		}
		return bytes;
	}

	public byte[] getBytes() {
		return bytes;
	}
}
