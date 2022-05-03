package edu.montana.csci.csci366.strweb.ops;

import java.util.concurrent.CountDownLatch;

/**
 * This class is should calculate the length of each line and replace the line in the
 * array with a string representation of its length
 */
public class LineLengthTransformer {
    String[] _lines;

    public LineLengthTransformer(String strings) {
        _lines = strings.split("\n|\r\n");
    }

    public String toLengths() {
        CountDownLatch latch = new CountDownLatch(_lines.length);
        for (int i = 0; i < _lines.length; i++){
            LineLengthCalculator lineLengthCalculator = new LineLengthCalculator(i, latch);
            new Thread(lineLengthCalculator).start();
        }
        try{
            latch.await();
        }catch(InterruptedException e){
            throw new RuntimeException(e);
        }
        return String.join("\n",_lines);
        //TODO - this method should create a series of Runnables and use Threads to do all
        //       line lengths in parallel
    }

    class LineLengthCalculator implements Runnable{
        private final int index;
        private final CountDownLatch latch;
        public LineLengthCalculator(int index, CountDownLatch latch){
            this.index = index;
            this.latch = latch;
        }
        @Override
        public void run(){
            String currentValue = _lines[index];
            _lines[index] = String.valueOf(currentValue.length());
            latch.countDown();
        }

    }

}