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

    public Sha256Transformer(String strings) {
        _lines = strings.split("\n");
    }

    public String toSha256Hashes() {
        CountDownLatch latch = new CountDownLatch(_lines.length);
        for (int i = 0; i < _lines.length; i++) {
            Sha256Computer sha256Computer = new Sha256Computer(i, latch);
            executor.execute(sha256Computer);
        }
        try {
            latch.await();
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        return String.join("\n", _lines);
    }

    class Sha256Computer implements Runnable {
        private final int index;
        private final CountDownLatch latch;

        public Sha256Computer(int index, CountDownLatch latch) {
            this.index = index;
            this.latch = latch;
        }

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