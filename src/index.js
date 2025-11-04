import { v7 as uuidv7 } from 'uuid';

const setAlarm = async (message, env, ctx) => {
  const uuid = uuidv7();
  const sender = message.from || "default_sender";

  try {
    const stmt = env.DATABASE.prepare(`
      INSERT INTO Alarms (uuid, sender, created_at, updated_at)
      VALUES (?, ?, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)
    `);
    await stmt.bind(uuid, sender).run();

  } catch (error) {
    console.error("Error creating alarm:", error);
  }
}

export default {
  async fetch(req, env, ctx) {
    if (req.method === 'POST') {
      console.log('setting the alarm');
      await setAlarm({}, env, ctx);
      return new Response("ok", {status: 200});
    } else {
      const result = await env.DATABASE.prepare(`
        SELECT *
        FROM Alarms
        WHERE deleted_at IS NULL
        ORDER BY created_at DESC
        LIMIT 1
      `)
        .first();
      console.log(result);
      if (result) {
        const createdAt = new Date(result.created_at); // Parse the created_at timestamp
        const now = new Date();
        const timeDifference = (now - createdAt) / 1000; // Difference in seconds
        if (timeDifference <= 30) {
          return new Response("1", { status: 200 });
        }
      }

      return new Response("0", { status: 200 });
    }
  },

  async email(message, env, ctx) {
    console.log(message.from);
    await setAlarm(message, env, ctx);
  }
}
