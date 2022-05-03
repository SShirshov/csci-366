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
        String[] split = _strings.split("\n|\r\n");
        Arrays.sort(split);
        return String.join("\n", split);
    }

    public String reverseSort() {
        String[] split = _strings.split("\n|\r\n");
        Arrays.sort(split, Collections.reverseOrder());
        return  String.join("\n", split);
    }

    public String parallelSort() {
        String[] split = _strings.split("\n|\r\n");
        Arrays.parallelSort(split);
        return  String.join("\n", split);
    }
}