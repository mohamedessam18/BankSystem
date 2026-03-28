import { Navigate, Route, Routes } from "react-router-dom";
import { useAuth } from "./context/AuthContext";
import LandingPage from "./pages/LandingPage";
import LoginPage from "./pages/LoginPage";
import SignupPage from "./pages/SignupPage";
import ClientDashboardPage from "./pages/ClientDashboardPage";
import ClientTransactionsPage from "./pages/ClientTransactionsPage";
import EmployeeDashboardPage from "./pages/EmployeeDashboardPage";
import EmployeeClientsPage from "./pages/EmployeeClientsPage";
import AdminDashboardPage from "./pages/AdminDashboardPage";
import AdminPeoplePage from "./pages/AdminPeoplePage";
import AdminSecurityPage from "./pages/AdminSecurityPage";
import ManagerDashboardPage from "./pages/ManagerDashboardPage";
import ManagerPeopleControlPage from "./pages/ManagerPeopleControlPage";
import ManagerExecutiveToolsPage from "./pages/ManagerExecutiveToolsPage";

function ProtectedRoute({ roles, children }) {
  const { session } = useAuth();

  if (!session) {
    return <Navigate to="/login" replace />;
  }

  if (!roles.includes(session.role)) {
    return <Navigate to={`/dashboard/${session.role}`} replace />;
  }

  return children;
}

export default function App() {
  const { session } = useAuth();

  return (
    <Routes>
      <Route path="/" element={<LandingPage />} />
      <Route path="/login" element={session ? <Navigate to={`/dashboard/${session.role}`} replace /> : <LoginPage />} />
      <Route path="/signup" element={session ? <Navigate to={`/dashboard/${session.role}`} replace /> : <SignupPage />} />
      <Route
        path="/dashboard/manager"
        element={
          <ProtectedRoute roles={["manager"]}>
            <ManagerDashboardPage />
          </ProtectedRoute>
        }
      />
      <Route
        path="/dashboard/manager/people-control"
        element={
          <ProtectedRoute roles={["manager"]}>
            <ManagerPeopleControlPage />
          </ProtectedRoute>
        }
      />
      <Route
        path="/dashboard/manager/executive-tools"
        element={
          <ProtectedRoute roles={["manager"]}>
            <ManagerExecutiveToolsPage />
          </ProtectedRoute>
        }
      />
      <Route
        path="/dashboard/client"
        element={
          <ProtectedRoute roles={["client"]}>
            <ClientDashboardPage />
          </ProtectedRoute>
        }
      />
      <Route
        path="/dashboard/client/transactions"
        element={
          <ProtectedRoute roles={["client"]}>
            <ClientTransactionsPage />
          </ProtectedRoute>
        }
      />
      <Route
        path="/dashboard/employee"
        element={
          <ProtectedRoute roles={["employee"]}>
            <EmployeeDashboardPage />
          </ProtectedRoute>
        }
      />
      <Route
        path="/dashboard/employee/clients"
        element={
          <ProtectedRoute roles={["employee"]}>
            <EmployeeClientsPage />
          </ProtectedRoute>
        }
      />
      <Route
        path="/dashboard/admin"
        element={
          <ProtectedRoute roles={["admin"]}>
            <AdminDashboardPage />
          </ProtectedRoute>
        }
      />
      <Route
        path="/dashboard/admin/people"
        element={
          <ProtectedRoute roles={["admin"]}>
            <AdminPeoplePage />
          </ProtectedRoute>
        }
      />
      <Route
        path="/dashboard/admin/security"
        element={
          <ProtectedRoute roles={["admin"]}>
            <AdminSecurityPage />
          </ProtectedRoute>
        }
      />
      <Route path="*" element={<Navigate to="/" replace />} />
    </Routes>
  );
}
