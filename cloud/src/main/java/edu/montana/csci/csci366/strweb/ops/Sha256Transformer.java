package edu.montana.csci.csci366.strweb.ops;

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

/**
 * This class is should calculate the SHA 256 hexadecimal hash for each line and replace the line in the
 * array with that hash.
 *
 * It should do so using the ThreadPoolExecutor created below.
 */
public class Sha256Transformer {
    String[] _lines;
    //create new executor that will be used to execute submitted tasks using one of in this case 10 pooled threads
    ThreadPoolExecutor executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(10);
    //get assign input
    public Sha256Transformer(String strings) {
        //split apart input into array of each line
        _lines = strings.split("\n");
    }

    public String toSha256Hashes() {//method creates runnables that get all sha256 hash codes from each line in parallel

        //create a countdown latch to ensure that our plaralization is synchronized aka each thread waits for a
        // certain number of threads in this case the value of lines.length before the main thread
        CountDownLatch latch = new CountDownLatch(_lines.length);

        //create new runable for each line and then assign each runnable to its own thread with the
        // threadpoolexecuter we made earlier assign each runnable a latch
        // latch being number of lines
        for (int i = 0; i < _lines.length; i++) {
            //create new runnable sha256Computer that we will assign to a thread
            Sha256Computer sha256Computer = new Sha256Computer(i, latch);
            //submmit task to executor which then uses a pooled thread to execute runnable
            executor.execute(sha256Computer);
        }
        //ensure that the function does not return before all threads have finished
        try {
            //make thread wait until current thread has counted down to zero
            latch.await();
        } catch (InterruptedException e) {//throw exception if thread is interupted while waiting, sleeping, or occupied
            throw new RuntimeException(e);
        }
        //join the split array back into on string and return string
        return String.join("\n", _lines);
    }

    class Sha256Computer implements Runnable {
        private final int index;
        private final CountDownLatch latch;

        public Sha256Computer(int index, CountDownLatch latch) {
            //store index of what we are transforming to sha256
            this.index = index;
            //store latch
            this.latch = latch;
        }
        //function to get sha256 code for line
        //carson says don't need to document details
        public void run() {
            MessageDigest digest = null;
            try {
                String originalString = _lines[index];
                digest = MessageDigest.getInstance("SHA-256");
                byte[] encodedhash = digest.digest(originalString.getBytes(StandardCharsets.UTF_8));
                _lines[index] = bytesToHex(encodedhash);
                latch.countDown();
            } catch (NoSuchAlgorithmException e) {
                throw new RuntimeException(e);
            }
        }
        // function to convert byte array back to hex
        //carson says don't need to document details
        private String bytesToHex(byte[] hash) {
            StringBuilder hexString = new StringBuilder(2 * hash.length);
            for (int i = 0; i < hash.length; i++) {
                String hex = Integer.toHexString(0xff & hash[i]);
                if (hex.length() == 1) {
                    hexString.append('0');
                }
                hexString.append(hex);
            }
            return hexString.toString();
        }
    }
}