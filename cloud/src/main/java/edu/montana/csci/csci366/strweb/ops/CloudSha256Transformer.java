package edu.montana.csci.csci366.strweb.ops;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URI;
import java.net.URLEncoder;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.*;

/**
 * This class is should pass the SHA 256 calculation through to the two listed NODES in
 * the SHA 256 cloud
 *
 * It should dispatch them via a POST with a `Content-Type` header set to `text/plain`
 *
 * It should dispatch the two requests concurrently and then merge them.
 *
 */
public class CloudSha256Transformer {

    List<String> NODES = Arrays.asList("http://localhost:8001", "http://localhost:8002");
    private final String _strings;

    public CloudSha256Transformer(String strings) {
        _strings = strings;
    }

    public String toSha256Hashes() {
        return "";
    }

}


