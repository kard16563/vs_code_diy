import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.List;

import jdbc_bean.Hero;
import jdbc_dao.HeroDAO;

@WebServlet(name = "HeroListServlet",urlPatterns = "/HeroListServlet")
public class HeroListServlet extends HttpServlet {


    protected void service(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        response.setContentType("text/html; charset=UTF-8");

        List<Hero> heros = new HeroDAO().list();

        StringBuffer sb = new StringBuffer();
        sb.append("<table align='center' border='1' cellspacing='0'>\r\n");
        sb.append("<tr><td>id</td><td>name</td><td>hp</td><td>damage</td></tr>\r\n");

        String trFormat = "<tr><td>%d</td><td>%s</td><td>%f</td><td>%d</td></tr>\r\n";

        int i=0;
        for (Hero hero : heros) {

            i=i+1;
            System.out.println("88888---->"+i);
            String tr = String.format(trFormat, hero.getId(), hero.getName(), hero.getHp(), hero.getDamage());
            sb.append(tr);
        }

        sb.append("</table>");

        response.getWriter().write(sb.toString());

    }


}
