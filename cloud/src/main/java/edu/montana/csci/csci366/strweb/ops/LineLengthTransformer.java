package edu.montana.csci.csci366.strweb.ops;

import java.util.concurrent.CountDownLatch;

/**
 * This class is should calculate the length of each line and replace the line in the
 * array with a string representation of its length
 */
public class LineLengthTransformer {
    String[] _lines;

    public LineLengthTransformer(String strings) {
        //split apart input string into each line
        _lines = strings.split("\n|\r\n");
    }

    public String toLengths() {//method creates a series of Runnables and uses Threads to do all line lengths in parallel

        //create a countdown latch to ensure that our plaralization is synchronized aka each thread waits for a
        // certain number of threads in this case the value of lines.length before the main thread
        CountDownLatch latch = new CountDownLatch(_lines.length);

        //create new runable for each line and then assign each runnable to its own thread with the
        // latch being number of lines
        for (int i = 0; i < _lines.length; i++){

            //create a new runnable linelengthcalculator aka new up a linelengthcalclulator
            LineLengthCalculator lineLengthCalculator = new LineLengthCalculator(i, latch);

            //create new thread assigning the specific runnable created before.
            new Thread(lineLengthCalculator).start();
        }
        //ensure that the function does not return before all threads have finished
        try{
            //make thread wait until current thread has counted down to zero
            latch.await();
        }catch(InterruptedException e){//throw exception if thread is interupted while waiting, sleeping, or occupied
            throw new RuntimeException(e);
        }
        //join the split array back into on string
        return String.join("\n",_lines);
    }
    //runnable class for use in our toLengths function which will be assigned to threads
    class LineLengthCalculator implements Runnable{
        private final int index;
        private final CountDownLatch latch;
        public LineLengthCalculator(int index, CountDownLatch latch){
            this.index = index;
            //store latch in calculator
            this.latch = latch;
        }
        @Override
        public void run(){
            //getting current string value of line
            String currentValue = _lines[index];
            //compute the length of the line
            _lines[index] = String.valueOf(currentValue.length());
            //when we're down with this process countdown latch by one in a thread safe manner
            latch.countDown();
        }

    }

}