import axios from "axios";

const baseURL = import.meta.env.VITE_API_BASE_URL || "http://localhost:8080";

const api = axios.create({
  baseURL,
  headers: {
    "Content-Type": "application/json"
  }
});

export const extractApiError = (error) =>
  error?.response?.data?.message || error.message || "Something went wrong.";

export const authApi = {
  login: (payload) => api.post("/login", payload),
  signup: (payload) => api.post("/signup", payload)
};

export const clientApi = {
  getBalance: (id) => api.get(`/client/${id}/balance`),
  deposit: (id, amount) => api.post(`/client/${id}/deposit`, { amount }),
  withdraw: (id, amount) => api.post(`/client/${id}/withdraw`, { amount }),
  transfer: (payload) => api.post("/client/transfer", payload)
};

export const employeeApi = {
  addClient: (payload) => api.post("/employee/add-client", payload),
  getClients: () => api.get("/employee/clients"),
  getClient: (id) => api.get(`/employee/client/${id}`),
  updateClient: (id, payload) => api.put(`/employee/client/${id}`, payload)
};

export const adminApi = {
  addEmployee: (payload) => api.post("/admin/add-employee", payload),
  getEmployees: () => api.get("/admin/employees"),
  updateEmployee: (id, payload) => api.put(`/admin/employee/${id}`, payload)
};

export const managerApi = {
  getOverview: () => api.get("/manager/overview"),
  getAdmins: () => api.get("/manager/admins"),
  addAdmin: (payload) => api.post("/manager/add-admin", payload),
  updateAdmin: (id, payload) => api.put(`/manager/admin/${id}`, payload)
};

export const notificationApi = {
  getNotifications: (role, userId) =>
    api.get(`/notifications?role=${encodeURIComponent(role)}${userId ? `&userId=${encodeURIComponent(userId)}` : ""}`),
  markRead: (role, userId) => api.post("/notifications/read", { role, userId })
};

export default api;
