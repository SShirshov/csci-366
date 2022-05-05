package edu.montana.csci.csci366.strweb.ops;

import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.Collections;

/**
 * This class is a simple sorter that implements sorting a strings rows in different ways
 */
public class Sorter {
    private final String _strings;

    public Sorter(String strings) {
        _strings = strings;
    }

    public String sort() {//method used to sort characters
        //break apart input string at every \n|\r\n putting all the
        // broken apart parts of string into an array called split
        String[] split = _strings.split("\n|\r\n");
        //use array classes sort function to sort our "split" array
        Arrays.sort(split);
        //join split array back into single string and return the string
        return String.join("\n", split);
    }

    public String reverseSort() {//method used to sort characters in reverse order
        //break apart input string at every \n|\r\n putting all the
        // broken apart parts of string into an array called split
        String[] split = _strings.split("\n|\r\n");
        //use array classes sort function to sort our "split" array and the collections libraries reverse order to
        // sort and then reverse the sorted array making the array sorted in reverse
        Arrays.sort(split, Collections.reverseOrder());
        //join split array back into single string and return the string
        return  String.join("\n", split);
    }

    public String parallelSort() {// method used to sort input string in parallel
        //break apart input string at every \n|\r\n putting all the
        // broken apart parts of string into an array called split
        String[] split = _strings.split("\n|\r\n");
        //use array classes parallel sort function to sort our "split" array by spliting it further into sub arrays
        // and sorting them before merging them again into one sorted array
        Arrays.parallelSort(split);
        //join split array back into single string and return the string
        return  String.join("\n", split);
    }
}