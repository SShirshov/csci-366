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
    //list of in this case two servers urls that we can assign tasks to
    List<String> NODES = Arrays.asList("http://localhost:8001", "http://localhost:8002");
    private final String _strings;

    public CloudSha256Transformer(String strings) {
        _strings = strings;
    }

    public String toSha256Hashes() {//method to convert each line to sha256 hash
        try {
            //take strings that are inputed split at each new line(\n) and then go
            // further and split each line as close to half as possible
            int index = _strings.indexOf("\n", _strings.length() / 2);

            //take first 1/2 or first chuck as we called the var
            String firstChunk = _strings.substring(0, index);
            //use http client to take first chunck and create POST
            var client = HttpClient.newHttpClient();
            //build new request POST that we send to server
            var request = HttpRequest.newBuilder()
                    //get first entry in array of cloud nodes or servers
                    .uri(URI.create(NODES.get(0)))
                    //assign headers of http request
                    .headers("Content-Type", "text/plain")
                    //set operator to be used + url-encoded version of what we want processed by operator
                    .POST(HttpRequest.BodyPublishers.ofString("op=Line+Sha256&strings=" + URLEncoder.encode(firstChunk, StandardCharsets.UTF_8.name())))
                    .build();
            //send POST for the first chunk to first server http://localhost:8001 with operation Line+Sha256
            HttpResponse<String> firstResponse = client.send(request, HttpResponse.BodyHandlers.ofString());

            //repeat the same thing for the second half aka secondchunk and send it to second node or second server
            String secondChunk = _strings.substring(index);
            //use http client to take second chunck and create POST
            var request2 = HttpRequest.newBuilder()
                    //get second entry in array of cloud nodes or servers
                    .uri(URI.create(NODES.get(1)))
                    //assign headers of http request
                    .headers("Content-Type", "text/plain")
                    //set operator to be used + url-encoded version of what we want processed by operator
                    .POST(HttpRequest.BodyPublishers.ofString("op=Line+Sha256&strings=" + URLEncoder.encode(secondChunk, StandardCharsets.UTF_8.name())))
                    .build();
            //send POST for the second chunk to second server http://localhost:8002 with operation Line+Sha256
            HttpResponse<String> secondResponse = client.send(request2, HttpResponse.BodyHandlers.ofString());

            // since normally the two requests we send are asynchronous and don't wait for one another we ask for
            // body in the return to ensure that the two block and wait for one another to finish
            //as we get both responses back we concatenate the two and return the result
            return firstResponse.body() + secondResponse.body();

        } catch (Exception e) {//throw exception if thread is interrupted while waiting, sleeping, or occupied
            throw new RuntimeException(e);
        }
    }
}


