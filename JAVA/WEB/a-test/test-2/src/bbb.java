import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

@WebServlet(name="bbb",urlPatterns = "/bbb")
public class bbb extends HttpServlet {
    private String message1;

    @Override
    public void init() throws ServletException {
        message1 = "bbb 666Hello world, this message is from servlet!";
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //设置响应内容类型  
        resp.setContentType("text/html");



        //设置逻辑实现  
        PrintWriter out = resp.getWriter();
        out.println("<h1>  " + message1 + "</h1>");
    }
    @Override
    public void destroy() {
        super.destroy();
    }
}
