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
        try {
            int index = _strings.indexOf("\n", _strings.length() / 2);

            String firstChunk = _strings.substring(0, index);
            var client = HttpClient.newHttpClient();
            var request = HttpRequest.newBuilder()
                    .uri(URI.create(NODES.get(0)))
                    .headers("Content-Type", "text/plain")
                    .POST(HttpRequest.BodyPublishers.ofString("op=Line+Sha256&strings=" + URLEncoder.encode(firstChunk, StandardCharsets.UTF_8.name())))
                    .build();
            HttpResponse<String> firstResponse = client.send(request, HttpResponse.BodyHandlers.ofString());

            String secondChunk = _strings.substring(index);
            var request2 = HttpRequest.newBuilder()
                    .uri(URI.create(NODES.get(1)))
                    .headers("Content-Type", "text/plain")
                    .POST(HttpRequest.BodyPublishers.ofString("op=Line+Sha256&strings=" + URLEncoder.encode(secondChunk, StandardCharsets.UTF_8.name())))
                    .build();
            HttpResponse<String> secondResponse = client.send(request2, HttpResponse.BodyHandlers.ofString());

            return firstResponse.body() + secondResponse.body();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}


