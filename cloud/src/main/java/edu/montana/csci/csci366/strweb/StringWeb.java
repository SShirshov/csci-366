package edu.montana.csci.csci366.strweb;

import edu.montana.csci.csci366.strweb.ops.CloudSha256Transformer;
import edu.montana.csci.csci366.strweb.ops.LineLengthTransformer;
import edu.montana.csci.csci366.strweb.ops.Sha256Transformer;
import edu.montana.csci.csci366.strweb.ops.Sorter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URLDecoder;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.stream.Collectors;

public class StringWeb {

    private static final Logger LOGGER = LoggerFactory.getLogger(StringWeb.class);
    private final int _port;
    public StringWeb(int port) {
        _port = port;
    }

    //====================================================================
    // HTTP Server
    //====================================================================

    private void run() throws IOException {
        ServerSocket socket = new ServerSocket(_port);
        LOGGER.info("Listening on http://localhost:" + _port);
        while (true) {
            Socket connection = socket.accept();
            LOGGER.info("Handling connection");
            handleConnection(connection);
        }
    }

    private void handleConnection(Socket connection) throws IOException {
        try (
                connection;
                InputStream inputStream = connection.getInputStream();
                OutputStream outputStream = connection.getOutputStream();
                PrintWriter outputWriter = new PrintWriter(outputStream);
                InputStreamReader in = new InputStreamReader(inputStream);
                BufferedReader inputReader = new BufferedReader(in);
        ) {
            long start = System.currentTimeMillis();
            StringBuilder response = new StringBuilder();
            try {
                // Determine request type & path
                String requestInfo = inputReader.readLine();
                LOGGER.info("Request Info: " + requestInfo);
                String[] requestInfoArr = requestInfo.split(" ");
                String method = requestInfoArr[0];
                String path = requestInfoArr[1];

                if ("/".equals(path)) {
                    // Parse HTTP Headers

                    Map<String, String> headers = new HashMap<>();
                    // TODO - parse request headers
                    String line;
                    do{
                        line = inputReader.readLine();
                        if(!line.isEmpty()){
                            String[] split = line.split(":",2);
//                            System.out.println(split[0]);
//                            System.out.println(split[1]);
                            headers.put(split[0],split[1].substring(1));
                        }

                    }while(!line.isEmpty());
                    LOGGER.info("Headers : " + headers);

                    String strings = "";
                    // TODO - parse request body
                    if("POST".equals(method)){
                        Map<String, String> parameters = new HashMap<>();
                        if(headers.containsKey("Content-Length")){
                            int length = Integer.parseInt((headers.get("Content-Length")));
                            char[] buffer = new char[length];
                            inputReader.read(buffer, 0, length);
                            String strBody = new String(buffer);
//                            System.out.println(strBody);
                            for (String param: strBody.split("&")){
                                String[] split = param.split("=",2);
                                String name = URLDecoder.decode(split[0], StandardCharsets.UTF_8);
                                String value = URLDecoder.decode(split[1], StandardCharsets.UTF_8);
                                parameters.put(name,value);

                            }
                        }
                        strings = parameters.get("strings");
                        if(parameters.containsKey("op")){
                            strings = doOperation(parameters.get("op"), strings);
                        }

                    }

                    // build a response
                    if ("text/plain".equals(headers.get("Content-Type"))) {
                        renderTextPlain(response, strings);
                    } else {
                        renderIndex(response, strings);
                    }
                } else {
                    render404(response, path);
                }
            } catch (Exception e) {
                renderErrorMessage(response, e);
            }
            LOGGER.info("Responding With : \n\n" + response);
            outputWriter.println(response);
            outputWriter.println("\n\n");
            outputWriter.flush();
            long end = System.currentTimeMillis();
            LOGGER.info("Response Time : " + (end - start) + " milliseconds");
        }
    }

    //====================================================================
    // Operation Dispatch
    //====================================================================

    private String doOperation(String op, String strings) {
        if (op.equals("Sort")) {
            Sorter sorter = new Sorter(strings);
            return sorter.sort();
        } else if (op.equals("Reverse Sort")) {
            Sorter sorter = new Sorter(strings);
            return sorter.reverseSort();
        } else if (op.equals("Parallel Sort")) {
            Sorter sorter = new Sorter(strings);
            return sorter.parallelSort();
        } else if (op.equals("Line Length")) {
            LineLengthTransformer sorter = new LineLengthTransformer(strings);
            return sorter.toLengths();
        } else if (op.equals("Line Sha256")) {
            Sha256Transformer wordCounder = new Sha256Transformer(strings);
            return wordCounder.toSha256Hashes();
        } else if (op.equals("Cloud Sha256")) {
            CloudSha256Transformer counter = new CloudSha256Transformer(strings);
            return counter.toSha256Hashes();
        } else {
            throw new UnsupportedOperationException("Don't know how to " + op);
        }
    }

    //====================================================================
    // Inline HTML templates
    //====================================================================

    public void renderIndex(StringBuilder response, String result) {
        renderResponseHeaders(response, "200 OK");
        response.append("<html><body><h1>Welcome to the StringWeb Webserver!</h1>\n")
                .append("<form action='/' method='post'>\n")
                .append("<h3>Enter Strings Below</h3>\n");

        response.append("<textarea rows='30' style='width:50%; margin:12px'name='strings'>\n");
        if (result != null) {
            response.append(result);
        }
        response.append("</textarea><br/>\n");

        response.append("<input type='submit' name='op' value='Sort'>\n")
                .append("<input type='submit' name='op' value='Reverse Sort'>\n")
                .append("<input type='submit' name='op' value='Parallel Sort'>\n")
                .append("<input type='submit' name='op' value='Line Length'>\n")
                .append("<input type='submit' name='op' value='Line Sha256'>\n")
                .append("<input type='submit' name='op' value='Cloud Sha256'>\n")
                .append("</form></body></html>");
    }

    private void renderTextPlain(StringBuilder response, String strings) {
        renderResponseHeaders(response, "200 OK");
        response.append(strings);
    }

    private void renderResponseHeaders(StringBuilder response, String responseVals) {
        response.append("HTTP/1.1 ").append(responseVals).append("\n")
                .append("MSU WebServer : 1.0\n")
                .append("Date : ").append(new Date()).append("\n").append("\n");
    }

    private void render404(StringBuilder response, String path) {
        response.setLength(0);
        renderResponseHeaders(response, "404 Not Found");
        response.append("<html><h3>The path ")
                .append(path)
                .append(" was not found</h3></html>");
    }

    private void renderErrorMessage(StringBuilder response, Exception e) {
        response.setLength(0);
        renderResponseHeaders(response, "500 ERROR");
        response.append("<html><h3>An Error Occured: ")
                .append(e.getMessage())
                .append("</h3><pre>")
                .append(Arrays.asList(e.getStackTrace()).stream().map(stackTraceElement -> stackTraceElement.toString()).collect(Collectors.joining("\n")))
                .append("</pre></html>");
    }

    //====================================================================
    // Boot Strap
    //====================================================================

    public static void main(String[] args) throws IOException {
        StringWeb server = new StringWeb(getPort());
        server.run();
    }

    protected static int getPort() {
        ProcessBuilder processBuilder = new ProcessBuilder();
        if (processBuilder.environment().get("PORT") != null) {
            return Integer.parseInt(processBuilder.environment().get("PORT"));
        }
        return 8000;
    }
}
