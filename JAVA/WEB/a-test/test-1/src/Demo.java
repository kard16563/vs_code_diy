
import javax.servlet.ServletException;

import javax.servlet.annotation.WebServlet;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;

import java.io.PrintWriter;



@WebServlet(name = "Demo",urlPatterns="/Demo")

public class Demo extends HttpServlet {

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {



    }



    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

        //设置网页响应类型

        response.setContentType("text/html");

        //实现具体操作

        PrintWriter out = response.getWriter();

        out.println("This is a new servlet page");
        out.close();

    }

}
